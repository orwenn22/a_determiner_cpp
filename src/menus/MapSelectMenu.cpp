#include "MapSelectMenu.h"

#include <filesystem>

#include "engine/state/StateManager.h"
#include "engine/util/Paths.h"
#include "engine/util/Open.h"
#include "engine/util/Paths.h"
#include "engine/util/Trace.h"
#include "engine/widgets/Label.h"
#include "engine/widgets/TiledButton.h"
#include "engine/widgets/TiledDivision.h"
#include "engine/widgets/WidgetManager.h"
#include "maploaders/LegacyMapLoader.h"
#include "utils/TiledBackground.h"
#include "GameplayState.h"
#include "GlobalResources.h"
#include "MainMenu.h"

#define MAP_PER_PAGES 6
#define MAP_BUTTON_HEIGHT 40
#define BUTTON_MARGE 10


MapSelectMenu::MapSelectMenu() : State() {
    m_error_label = nullptr;
    m_loaders.push_back(new LegacyMapLoader);

    m_bg = new TiledBackground(&Res::menubg_sprite);

    m_permanent_widgets = new WidgetManager;
    m_temporary_widgets = new WidgetManager;
    m_cwd_label = nullptr;

    m_current_page_label = new Label(-20, 5, 20, "0");
    m_current_page_label->SetAlignment(WidgetAlignment_MiddleBottom);
    m_current_page_label->SetOutline(true);
    m_permanent_widgets->AddWidget(m_current_page_label);

    m_page_count_label = new Label(20, 5, 20, "0");
    m_page_count_label->SetAlignment(WidgetAlignment_MiddleBottom);
    m_page_count_label->SetOutline(true);
    m_permanent_widgets->AddWidget(m_page_count_label);

    Label *slash = new Label(0, 5, 20, "/");
    slash->SetAlignment(WidgetAlignment_MiddleBottom);
    slash->SetOutline(true);
    m_permanent_widgets->AddWidget(slash);

    UpdateMapNames();

    TiledButton *back_button = new TiledButton(0, 10, 150, 40, &Res::tiled_button_right_sprite, 8, 2, "Back");
    back_button->SetFontSize(20);
    back_button->CenterLabel();
    back_button->SetCallback([=]() {
        this->Manager()->SetState(new MainMenu, true);
    });
    m_permanent_widgets->AddWidget(back_button);

    TiledButton *next_button = new TiledButton(100, 30, 150, 40, &Res::tiled_button_sprite, 8, 2, "Next");
    next_button->SetAlignment(WidgetAlignment_MiddleBottom);
    next_button->SetFontSize(20);
    next_button->CenterLabel();
    next_button->SetCallback([=]() {
        SetPage(this->m_page+1);
    });
    m_permanent_widgets->AddWidget(next_button);

    TiledButton *prev_button = new TiledButton(-100, 30, 150, 40, &Res::tiled_button_sprite, 8, 2, "Prev");
    prev_button->SetAlignment(WidgetAlignment_MiddleBottom);
    prev_button->SetFontSize(20);
    prev_button->CenterLabel();
    prev_button->SetCallback([=]() {
        SetPage(this->m_page-1);
    });
    m_permanent_widgets->AddWidget(prev_button);

    TiledButton *reload_button = new TiledButton(0, 80, 185, 40, &Res::tiled_button_left_sprite, 8, 2, "Reload");
    reload_button->SetAlignment(WidgetAlignment_Right);
    reload_button->SetFontSize(20);
    reload_button->CenterLabel();
    reload_button->SetCallback([this]() { this->UpdateMapNames(); });
    m_permanent_widgets->AddWidget(reload_button);

    //Display CWD
    std::string cwd = GetCWD();
    if(cwd.empty()) return;

    m_cwd_label = new Label(10, 10, 20, "Map folder : [HOLD F]");
    m_cwd_label->SetAlignment(WidgetAlignment_Right);
    m_cwd_label->SetOutline(true);
    m_permanent_widgets->AddWidget(m_cwd_label);

    TiledButton *open_map_folder = new TiledButton(0, 35, 185, 40, &Res::tiled_button_left_sprite, 8, 2, "Open map folder");
    open_map_folder->SetAlignment(WidgetAlignment_Right);
    open_map_folder->SetFontSize(20);
    open_map_folder->CenterLabel();
    open_map_folder->SetCallback([cwd]() { OpenWithDefault((cwd + "/maps").c_str()); });
    m_permanent_widgets->AddWidget(open_map_folder);
}

MapSelectMenu::~MapSelectMenu() {
    delete m_temporary_widgets;
    delete m_permanent_widgets;
    delete m_bg;

    for(MapLoader *l : m_loaders) delete l;
    m_loaders.clear();
}

void MapSelectMenu::Update(float dt) {
    if(m_cwd_label != nullptr) {
        if(IsKeyPressed(KEY_F)) m_cwd_label->SetLabel("Map folder : " + GetCWD() + "/maps");
        else if(IsKeyReleased(KEY_F)) m_cwd_label->SetLabel("Map folder : [HOLD F]");
    }
    m_permanent_widgets->Update();
    m_temporary_widgets->Update();
    m_bg->Update(dt);
}

void MapSelectMenu::Draw() {
    m_bg->Draw();
    m_temporary_widgets->Draw();
    m_permanent_widgets->Draw();
}


/////////////////////////////////
//// PRIVATE

void MapSelectMenu::UpdateMapNames() {
    m_map_names.clear();
    for(auto element : std::filesystem::directory_iterator("./maps")) {
        if(!element.is_regular_file()) continue;
        m_map_names.push_back(element.path());
    }
    size_t map_count = m_map_names.size();
    m_page_count = (int) (map_count / MAP_PER_PAGES + (map_count%MAP_PER_PAGES != 0));
    m_page_count_label->SetLabel(std::to_string(m_page_count));
    SetPage(0);
}

void MapSelectMenu::SetPage(int page) {
    m_temporary_widgets->Clear();

    m_page = page;
    if(m_page < 0) m_page = 0;
    if(m_page >= m_page_count) m_page = m_page_count-1;
    m_current_page_label->SetLabel(std::to_string(m_page+1));
    TRACE("Page is now %i\n", m_page);

    int total_height = MAP_BUTTON_HEIGHT*MAP_PER_PAGES + BUTTON_MARGE*(MAP_PER_PAGES-1);     //40 = button height, 10 = marge
    int start_y = -total_height/2 + MAP_BUTTON_HEIGHT/2;

    int i_stop = MAP_PER_PAGES * (m_page+1);
    for(size_t i = MAP_PER_PAGES*m_page; i < i_stop; ++i) {
        if(i >= m_map_names.size()) {
            TRACE("Noting to do\n");
            TiledDivision *td = new TiledDivision(0, start_y, 250, MAP_BUTTON_HEIGHT, &Res::empty_outline_sprite, 8, 2);
            td->SetAlignment(WidgetAlignment_Center);
            td->SetColor({200, 200, 200, 100});
            m_temporary_widgets->AddWidget(td);
            start_y += MAP_BUTTON_HEIGHT+BUTTON_MARGE;
            continue;
        }

        TRACE("Button for %s\n", m_map_names[i].c_str());
        std::string file_name_only = GetFileFromPath(m_map_names[i]);
        TiledButton *b = new TiledButton(0, start_y, 250, MAP_BUTTON_HEIGHT, &Res::tiled_button_sprite, 8, 2, file_name_only);
        b->SetAlignment(WidgetAlignment_Center);
        b->SetFontSize(20);
        b->CenterLabel();
        b->SetCallback([this, i]() {
            TRACE("loading level %s\n", m_map_names[i].c_str());
            this->LoadMap((int)i);
        });
        m_temporary_widgets->AddWidget(b);

        start_y += MAP_BUTTON_HEIGHT+BUTTON_MARGE;
    }
}

void MapSelectMenu::LoadMap(int index) {
    if(index >= m_map_names.size()) {
        TRACE("index %i out of range\n", index);
        return;
    }

    int loader_to_use = -1;
    for(int i = 0; i < m_loaders.size(); ++i) {
        if(m_loaders[i]->IsCorrectFormat(m_map_names[index])) {
            loader_to_use = i;
            break;
        }
    }

    if(loader_to_use == -1) {
        TRACE("Could not find appropriate map loader for map %s\n", m_map_names[index].c_str());
        SetError("Could not find appropriate map loader for map " + m_map_names[index]);
        return;
    }

    TRACE("using map loader %i\n", loader_to_use);

    ErrorOr<GameplayState *> gs = m_loaders[loader_to_use]->LoadMap(m_map_names[index]);
    if(gs.GetError() != nullptr) {
        TRACE("failed to load map for index %i (%s)\n", index, m_map_names[index].c_str());
        TRACE(" error message : %s\n", gs.GetError()->m_error_message.c_str());
        SetError(gs.GetError()->m_error_message);
        return;
    }

    Manager()->SetState(gs.GetValue(), true);
}


void MapSelectMenu::SetError(std::string error_message) {
    if (m_error_label != nullptr) {
        m_permanent_widgets->RemoveWidget(m_error_label);
        delete m_error_label;
    }

    m_error_label = new Label(0, 70, 20, error_message);
    m_error_label->SetAlignment(WidgetAlignment_HCenter);
    m_error_label->SetOutline(true);
    m_error_label->SetColor(RED);
    m_permanent_widgets->AddWidget(m_error_label);
}
