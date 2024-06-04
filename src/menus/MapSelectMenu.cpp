#include "MapSelectMenu.h"

#include <filesystem>

#include "engine/state/StateManager.h"
#include "engine/util/Trace.h"
#include "engine/widgets/Label.h"
#include "engine/widgets/TiledButton.h"
#include "engine/widgets/WidgetManager.h"
#include "maploaders/LegacyMapLoader.h"
#include "utils/TiledBackground.h"
#include "GameplayState.h"
#include "GlobalResources.h"
#include "MainMenu.h"

#define MAP_PER_PAGES 4


MapSelectMenu::MapSelectMenu() : State() {
    m_bg = new TiledBackground(&Res::menubg_sprite);

    m_permanent_widgets = new WidgetManager;
    m_temporary_widgets = new WidgetManager;

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
}

MapSelectMenu::~MapSelectMenu() {
    delete m_temporary_widgets;
    delete m_permanent_widgets;
    delete m_bg;
}

void MapSelectMenu::Update(float dt) {
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

    int total_height = 40*MAP_PER_PAGES + 10*(MAP_PER_PAGES-1);     //40 = button height, 10 = marge
    int start_y = -total_height/2 + 40/2;

    int i_stop = MAP_PER_PAGES * (m_page+1);
    for(size_t i = MAP_PER_PAGES*m_page; i < i_stop; ++i) {
        if(i >= m_map_names.size()) {
            TRACE("Noting to do\n");
            return;
        }

        TRACE("Button for %s\n", m_map_names[i].c_str());
        TiledButton *b = new TiledButton(0, start_y, 250, 40, &Res::tiled_button_sprite, 8, 2, m_map_names[i]);
        b->SetAlignment(WidgetAlignment_Center);
        b->SetFontSize(20);
        b->CenterLabel();
        b->SetCallback([=]() {
            TRACE("loading level %s\n", m_map_names[i].c_str());
            this->LoadMap((int)i);
        });
        m_temporary_widgets->AddWidget(b);

        start_y += 40+10;
    }
}

void MapSelectMenu::LoadMap(int index) {
    if(index >= m_map_names.size()) {
        TRACE("index %i out of range\n", index);
        return;
    }

    //TODO : support multiple map loader here
    LegacyMapLoader loader;
    GameplayState *gs = loader.LoadMap(m_map_names[index]);
    if(gs == nullptr) {
        TRACE("failed to load map for index %i (%s)", index, m_map_names[index].c_str());
    }

    Manager()->SetState(gs, true);
}
