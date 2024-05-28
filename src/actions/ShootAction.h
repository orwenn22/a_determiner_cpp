#ifndef UI_FRAMEWORK_SHOOTACTION_H
#define UI_FRAMEWORK_SHOOTACTION_H

#include "Action.h"

class ShootAction : public Action {
public:
    ShootAction();

    void OnUpdate(Player *player, float dt) override;
    void OnDraw(Player *player) override;
};


#endif //UI_FRAMEWORK_SHOOTACTION_H
