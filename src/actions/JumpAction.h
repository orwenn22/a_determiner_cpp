#ifndef UI_FRAMEWORK_JUMPACTION_H
#define UI_FRAMEWORK_JUMPACTION_H

#include "Action.h"

class JumpAction : public Action {
public:
    JumpAction();

    void OnUpdate(Player *player, float dt) override;
    void OnDraw(Player *player) override;
};


#endif //UI_FRAMEWORK_JUMPACTION_H
