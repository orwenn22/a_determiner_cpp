#include "PostGameMenu.h"

#include "engine/state/StateManager.h"
#include "engine/widgets/Label.h"
#include "engine/widgets/TiledButton.h"
#include "engine/widgets/WidgetManager.h"
#include "utils/TiledBackground.h"
#include "GlobalResources.h"
#include "MainMenu.h"
#include "Teams.h"

PostGameMenu::PostGameMenu(int winning_team) : State() {
    m_bg = new TiledBackground(&Res::menubg_grayscale);
    m_bg->SetColor(s_team_colors[winning_team]);

    m_widgets = new WidgetManager;

    Label *victory_label = new Label(0, -60, 30, std::string("Victory of ") + s_team_names[winning_team] + " team");
    victory_label->SetAlignment(WidgetAlignment_Center);
    victory_label->SetOutline(true);
    m_widgets->AddWidget(victory_label);

    TiledButton *back_to_menu = new TiledButton(0, 60, 250, 40, &Res::tiled_button_sprite, 8, 2, "Return to main menu");
    back_to_menu->SetAlignment(WidgetAlignment_Center);
    back_to_menu->SetFontSize(20);
    back_to_menu->CenterLabel();
    back_to_menu->SetCallback([=]() { Manager()->SetState(new MainMenu, true); });
    m_widgets->AddWidget(back_to_menu);
}

PostGameMenu::~PostGameMenu() {
    delete m_widgets;
    delete m_bg;
}

void PostGameMenu::Update(float dt) {
    m_widgets->Update();
    m_bg->Update(dt);
}

void PostGameMenu::Draw() {
    m_bg->Draw();
    m_widgets->Draw();
}
