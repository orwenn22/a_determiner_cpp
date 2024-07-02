#ifndef A_DETERMINER_POTATOACTION_H
#define A_DETERMINER_POTATOACTION_H

#include "Action.h"

class PotatoAction : public Action {
public:
    PotatoAction();

    void OnClick(Player *p, int action_index) override;
    bool OnSkip(Player *p) override;
};


#endif //A_DETERMINER_POTATOACTION_H
