#ifndef UI_FRAMEWORK_PLAYERINDICATOR_H
#define UI_FRAMEWORK_PLAYERINDICATOR_H

#include "engine/widgets/Widget.h"

class GameplayState;

class PlayerIndicator : public Widget {
public:
    PlayerIndicator(GameplayState *gameplay_state);
    ~PlayerIndicator() override;

    void Update() override;
    void Draw() override;


private:
    GameplayState *m_gameplay_state;
};


#endif //UI_FRAMEWORK_PLAYERINDICATOR_H
