#ifndef A_DETERMINER_SHOOTACTION_H
#define A_DETERMINER_SHOOTACTION_H

#include "Action.h"

class ShootAction : public Action {
public:
    ShootAction();

    void OnUpdate(Player *player, float dt) override;
    void OnDraw(Player *player) override;
};


#endif //A_DETERMINER_SHOOTACTION_H
