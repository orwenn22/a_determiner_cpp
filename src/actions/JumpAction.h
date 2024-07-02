#ifndef A_DETERMINER_JUMPACTION_H
#define A_DETERMINER_JUMPACTION_H

#include "Action.h"

class JumpAction : public Action {
public:
    JumpAction();

    void OnUpdate(Player *player, float dt) override;
    void OnDraw(Player *player) override;
};


#endif //A_DETERMINER_JUMPACTION_H
