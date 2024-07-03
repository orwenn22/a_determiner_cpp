#ifndef A_DETERMINER_MAINMENU_H
#define A_DETERMINER_MAINMENU_H

#include "engine/state/State.h"

class KeyboardCode;
class TiledBackground;
class WidgetManager;

class MainMenu : public State {
public:
    MainMenu();
    ~MainMenu() override;

    void Update(float dt) override;
    void Draw() override;

    inline const char *GetIdentifier() override { return "MainMenu"; }
    inline TiledBackground *BG() { return m_bg; }


private:
    TiledBackground *m_bg;
    WidgetManager *m_widgets;
    KeyboardCode *m_silly;
};


#endif //A_DETERMINER_MAINMENU_H
