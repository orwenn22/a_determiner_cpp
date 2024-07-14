#include "PauseMenu.h"

#include <raylib.h>

#include "editor/EditorState.h"
#include "engine/Globals.h"
#include "engine/state/StateManager.h"
#include "engine/widgets/Label.h"
#include "engine/widgets/TiledButton.h"
#include "engine/widgets/WidgetManager.h"
#include "utils/TiledBackground.h"
#include "GameplayState.h"
#include "GlobalResources.h"
#include "MainMenu.h"


PauseMenu::PauseMenu(GameplayState *gameplay_state) {
    m_gameplay_state = gameplay_state;
    m_bg = new TiledBackground(&Res::menubg_grayscale);
    m_bg->SetColor({45, 45, 45, 100});
    m_widgets = new WidgetManager;

    Label *pause_text = new Label(0, -55, 20, "== PAUSE ==");
    pause_text->SetAlignment(WidgetAlignment_Center);
    pause_text->SetOutline(true);
    m_widgets->AddWidget(pause_text);

    TiledButton *continue_button = new TiledButton(0, -10, 150, 40, &Res::tiled_button_sprite, 8, 2, "Resume");
    continue_button->SetAlignment(WidgetAlignment_Center);
    continue_button->SetFontSize(20);
    continue_button->CenterLabel();
    continue_button->SetCallback([this]() { ExitPause(); });
    m_widgets->AddWidget(continue_button);

    bool have_editor = m_gameplay_state->GetEditor();
    std::string quit_button_label = (have_editor) ? "Editor" : "Main menu";
    TiledButton *quit_button = new TiledButton(0, 40, 150, 40, &Res::tiled_button_sprite, 8, 2, quit_button_label);
    quit_button->SetAlignment(WidgetAlignment_Center);
    quit_button->SetFontSize(20);
    quit_button->CenterLabel();
    quit_button->SetHoverColor(have_editor ? BLUE : RED);
    quit_button->SetCallback([this]() {
        if(m_gameplay_state->GetEditor() == nullptr) {
            Manager()->SetState(new MainMenu, true);
        }
        else {
            Manager()->SetState(m_gameplay_state->GetEditor(), true);
            m_gameplay_state->SetEditor(nullptr);
        }
    });
    m_widgets->AddWidget(quit_button);
}

PauseMenu::~PauseMenu() {
    delete m_widgets;
    delete m_bg;
    delete m_gameplay_state;
}

void PauseMenu::Update(float dt) {
    if(IsKeyPressed(KEY_ESCAPE)) {
        ExitPause();
        return;
    }

    m_widgets->Update();
    m_bg->Update(dt);

    //Do this so the widgets of the gameplay stay centered even if the window is resized
    if(!IsMouseUsed()) UseMouse();
    if(m_gameplay_state == nullptr) return;
    m_gameplay_state->ForceUpdateWidgets();
}

void PauseMenu::Draw() {
    m_gameplay_state->Draw();
    //DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), {0, 0, 0, 100});
    m_bg->Draw();
    m_widgets->Draw();
}


///////////////////////////////////
//// PRIVATE

void PauseMenu::ExitPause() {
    Manager()->SetState(m_gameplay_state, true);
    m_gameplay_state = nullptr;
}
