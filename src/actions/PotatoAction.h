#ifndef UI_FRAMEWORK_POTATOACTION_H
#define UI_FRAMEWORK_POTATOACTION_H

#include "Action.h"

class PotatoAction : public Action {
public:
    PotatoAction();

    void OnClick(Player *p, int action_index) override;
    bool OnSkip(Player *p) override;
};


#endif //UI_FRAMEWORK_POTATOACTION_H
