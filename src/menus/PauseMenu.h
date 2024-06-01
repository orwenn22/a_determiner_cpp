#ifndef UI_FRAMEWORK_PAUSEMENU_H
#define UI_FRAMEWORK_PAUSEMENU_H

#include "engine/state/State.h"

class GameplayState;
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
    WidgetManager *m_widgets;
};


#endif //UI_FRAMEWORK_PAUSEMENU_H
