#ifndef A_DETERMINER_PAUSEMENU_H
#define A_DETERMINER_PAUSEMENU_H

#include "engine/state/State.h"

class GameplayState;
class TiledBackground;
class WidgetManager;

class PauseMenu : public State {
public:
    PauseMenu(GameplayState *gameplay_state);
    ~PauseMenu() override;

    void Update(float dt) override;
    void Draw() override;

    inline const char *GetIdentifier() override { return "PauseMenu"; }

private:
    void ExitPause();

    GameplayState *m_gameplay_state;
    TiledBackground *m_bg;
    WidgetManager *m_widgets;
};


#endif //A_DETERMINER_PAUSEMENU_H
