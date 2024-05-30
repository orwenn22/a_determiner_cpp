#ifndef UI_FRAMEWORK_FAKEACTIONWIDGET_H
#define UI_FRAMEWORK_FAKEACTIONWIDGET_H

#include "engine/widgets/TiledButton.h"

class Player;

class FakeActionWidget : public TiledButton {
public:
    FakeActionWidget(std::string action_name, std::string price_label = "(+10)", std::function<void(void)> callback = DefaultClickableCallback);

    void Draw() override;

protected:
    std::string m_price_label;
};


#endif //UI_FRAMEWORK_FAKEACTIONWIDGET_H
