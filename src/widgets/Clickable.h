#ifndef UI_FRAMEWORK_CLICKABLE_H
#define UI_FRAMEWORK_CLICKABLE_H

#include "Widget.h"

#include <functional>

class Clickable;
void DefaultClickableCallback();

//The goal of this is to provide the code for all the clickable widgets
class Clickable : public Widget {
public:
    Clickable(int x, int y, int w, int h, std::function<void(void)> callback = DefaultClickableCallback);

    void Update() override;

    void SetCallback(std::function<void(void)> callback);

protected:
    std::function<void(void)> m_callback;
    bool m_hovered;
};


#endif //UI_FRAMEWORK_CLICKABLE_H
