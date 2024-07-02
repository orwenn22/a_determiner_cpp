#ifndef A_DETERMINER_ACTIONWIDGET_H
#define A_DETERMINER_ACTIONWIDGET_H

#include "FakeActionWidget.h"

class Action;

class ActionWidget : public FakeActionWidget {
public:
    ActionWidget(Player *p, int action_index);

    void Draw() override;

private:
    Player *m_player;
    int m_index;
    int m_saved_price;
};


#endif //A_DETERMINER_ACTIONWIDGET_H
