#include "PauseMenu.h"

#include <raylib.h>

#include "engine/Globals.h"
#include "engine/state/StateManager.h"
#include "engine/widgets/Label.h"
#include "engine/widgets/TiledButton.h"
#include "engine/widgets/WidgetManager.h"
#include "GameplayState.h"
#include "GlobalResources.h"
#include "MainMenu.h"


PauseMenu::PauseMenu(GameplayState *gameplay_state) {
    m_gameplay_state = gameplay_state;
    m_widgets = new WidgetManager;

    Label *pause_text = new Label(0, -55, 20, "== PAUSE ==");
    pause_text->SetAlignment(WidgetAlignment_Center);
    pause_text->SetOutline(true);
    m_widgets->AddWidget(pause_text);

    TiledButton *continue_button = new TiledButton(0, -10, 150, 40, &Res::tiled_button_sprite, 8, 2, "Resume");
    continue_button->SetAlignment(WidgetAlignment_Center);
    continue_button->SetFontSize(20);
    continue_button->CenterLabel();
    continue_button->SetCallback([=]() { ExitPause(); });
    m_widgets->AddWidget(continue_button);

    TiledButton *main_menu_button = new TiledButton(0, 40, 150, 40, &Res::tiled_button_sprite, 8, 2, "Main menu");
    main_menu_button->SetAlignment(WidgetAlignment_Center);
    main_menu_button->SetFontSize(20);
    main_menu_button->CenterLabel();
    main_menu_button->SetHoverColor(RED);
    main_menu_button->SetCallback([=]() { Manager()->SetState(new MainMenu, true); });
    m_widgets->AddWidget(main_menu_button);
}

PauseMenu::~PauseMenu() {
    delete m_widgets;
    delete m_gameplay_state;
}

void PauseMenu::Update(float dt) {
    if(IsKeyPressed(KEY_ESCAPE)) {
        ExitPause();
        return;
    }

    m_widgets->Update();
    if(!IsMouseUsed()) UseMouse();
}

void PauseMenu::Draw() {
    m_gameplay_state->Draw();
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), {0, 0, 0, 100});
    m_widgets->Draw();
}


///////////////////////////////////
//// PRIVATE

void PauseMenu::ExitPause() {
    Manager()->SetState(m_gameplay_state, true);
    m_gameplay_state = nullptr;
}
