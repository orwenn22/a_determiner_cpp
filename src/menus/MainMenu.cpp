#include "MainMenu.h"

#include "engine/Globals.h"
#include "engine/state/StateManager.h"
#include "engine/util/Trace.h"
#include "engine/widgets/Label.h"
#include "engine/widgets/TiledButton.h"
#include "engine/widgets/WidgetManager.h"
#include "utils/TiledBackground.h"
#include "CreditsMenu.h"
#include "GameplayState.h"
#include "GlobalResources.h"
#include "OptionsMenu.h"


#ifdef GIT_COMMIT_HASH
static const char *s_git_commit_hash = GIT_COMMIT_HASH;
#else
static const char *s_git_commit_hash = "Unknown";
#endif

#ifdef GIT_BRANCH
static const char *s_git_branch = GIT_BRANCH;
#else
static const char *s_git_branch = "Unknown";
#endif


MainMenu::MainMenu() : State() {
    m_bg = new TiledBackground(&Res::menubg_sprite);
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

    //PLAY BUTTON
    TiledButton *play_button = new TiledButton(0, -50, 150, 40,
                                               &Res::tiled_button_sprite, 8, 2, "Play");
    play_button->SetFontSize(20);
    play_button->CenterLabel();
    play_button->SetAlignment(WidgetAlignment_Center);
    play_button->SetCallback([=]() {
        this->Manager()->SetState(new GameplayState);
    });
    m_widgets->AddWidget(play_button);

    //OPTIONS BUTTON
    TiledButton *option_button = new TiledButton(0, 0, 150, 40,
                                                 &Res::tiled_button_sprite, 8, 2, "Options");
    option_button->SetFontSize(20);
    option_button->CenterLabel();
    option_button->SetAlignment(WidgetAlignment_Center);
    option_button->SetCallback([=]() {
        this->Manager()->SetState(new OptionsMenu);
    });
    m_widgets->AddWidget(option_button);

    //CREDITS BUTTON
    TiledButton *credits_button = new TiledButton(0, 50, 150, 40,
                                                  &Res::tiled_button_sprite, 8, 2, "Credits");
    credits_button->SetFontSize(20);
    credits_button->CenterLabel();
    credits_button->SetAlignment(WidgetAlignment_Center);
    credits_button->SetCallback([=]() {
        this->Manager()->SetState(new CreditsMenu);
    });
    m_widgets->AddWidget(credits_button);

    //QUIT BUTTON
    TiledButton *quit_button = new TiledButton(0, 100, 150, 40,
                                               &Res::tiled_button_sprite, 8, 2, "Quit");
    quit_button->SetFontSize(20);
    quit_button->SetHoverColor(RED);
    quit_button->CenterLabel();
    quit_button->SetAlignment(WidgetAlignment_Center);
    quit_button->SetCallback([=]() {
        RequestQuit();
    });
    m_widgets->AddWidget(quit_button);


    Label *version_label = new Label(2, 2, 10, std::string(s_git_branch) + "@" + std::string(s_git_commit_hash).substr(0, 7));
    version_label->SetAlignment(WidgetAlignment_Bottom);
    version_label->SetOutline(true);
    m_widgets->AddWidget(version_label);
}

MainMenu::~MainMenu() {
    delete m_widgets;
    delete m_bg;
}

void MainMenu::Update(float dt) {
    m_widgets->Update();
    m_bg->Update(dt);
}

void MainMenu::Draw() {
    m_bg->Draw();
    m_widgets->Draw();
}
