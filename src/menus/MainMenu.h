#ifndef UI_FRAMEWORK_MAINMENU_H
#define UI_FRAMEWORK_MAINMENU_H

#include "engine/state/State.h"

class TiledBackground;
class WidgetManager;

class MainMenu : public State {
public:
    MainMenu();
    ~MainMenu() override;

    void Update(float dt) override;
    void Draw() override;

    inline const char *GetIdentifier() override { return "MainMenu"; }


private:
    TiledBackground *m_bg;
    WidgetManager *m_widgets;
};


#endif //UI_FRAMEWORK_MAINMENU_H
