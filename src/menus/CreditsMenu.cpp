#include "CreditsMenu.h"

#include "engine/state/StateManager.h"
#include "engine/widgets/Label.h"
#include "engine/widgets/TiledButton.h"
#include "engine/widgets/WidgetManager.h"
#include "engine/widgets/Widget.h"
#include "utils/TiledBackground.h"
#include "GlobalResources.h"
#include "MainMenu.h"


CreditsMenu::CreditsMenu() {
    m_bg = new TiledBackground(&Res::menubg_credits_sprite);

    m_widgets = new WidgetManager;

    //TITLE LABEL
    Label *title = new Label(0, -150, 40, "À déterminer");
    title->SetAlignment(WidgetAlignment_Center);
    title->SetOutline(true);
    title->SetColor({255, 255, 255, 255});
    title->SetOutlineColor({0, 0, 0, 255});
    m_widgets->AddWidget(title);

    //TM
    Label *tm = new Label(140, -160, 10, "TM");
    tm->SetAlignment(WidgetAlignment_Center);
    tm->SetOutline(true);
    tm->SetColor({255, 255, 255, 255});
    tm->SetOutlineColor({0, 0, 0, 255});
    m_widgets->AddWidget(tm);

    //By SomeGroup
    Label *some_group = new Label(0, -100, 30, "By : SomeGroup");
    some_group->SetAlignment(WidgetAlignment_Center);
    some_group->SetOutline(true);
    some_group->SetColor({255, 255, 255, 255});
    some_group->SetOutlineColor({0, 0, 0, 255});
    m_widgets->AddWidget(some_group);

    const char *names[] = {
            "orwenn22 - Programming & C++ remake",
            "DAlexis1 - Programming",
            "Applejude - Graphics",
            " ",
            "Explosion animation from deltarune"
    };
    size_t name_count = sizeof(names) / sizeof(char *);
    int y = -60;
    for(size_t i = 0; i < name_count; ++i) {
        Label *name = new Label(0, y, 20, names[i]);
        name->SetAlignment(WidgetAlignment_Center);
        name->SetOutline(true);
        name->SetColor({255, 255, 255, 255});
        name->SetOutlineColor({0, 0, 0, 255});
        m_widgets->AddWidget(name);
        y += 30;
    }

    TiledButton *back_button = new TiledButton(0, 200, 250, 40, &Res::tiled_button_sprite, 8, 2, "Return to main menu");
    back_button->SetAlignment(WidgetAlignment_Center);
    back_button->SetFontSize(20);
    back_button->CenterLabel();
    back_button->SetCallback([=]() { Manager()->SetState(new MainMenu, true); });
    m_widgets->AddWidget(back_button);
}

CreditsMenu::~CreditsMenu() {
    delete m_widgets;
    delete m_bg;
}

void CreditsMenu::Update(float dt) {
    m_widgets->Update();
    m_bg->Update(dt);
}

void CreditsMenu::Draw() {
    m_bg->Draw();
    m_widgets->Draw();
}
