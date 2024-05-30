#ifndef UI_FRAMEWORK_ACTIONWIDGET_H
#define UI_FRAMEWORK_ACTIONWIDGET_H

#include "FakeActionWidget.h"

class ActionWidget : public FakeActionWidget {
public:
    ActionWidget(Player *p, int action_index);

    void Draw() override;

private:
    Player *m_player;
    int m_index;
};


#endif //UI_FRAMEWORK_ACTIONWIDGET_H
