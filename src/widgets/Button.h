#ifndef UI_FRAMEWORK_BUTTON_H
#define UI_FRAMEWORK_BUTTON_H

#include "Widget.h"

#include <functional>
#include <string>

class Button;

void DefaultButtonCallback(Button *button);

class Button : public Widget {
public:
    Button(int x, int y, int w, int h, std::string label = "button", std::function<void(Button *)> callback = DefaultButtonCallback);

    void Update() override;
    void Draw() override;

    void SetLabel(std::string label);
    void SetCallback(std::function<void(Button *)> callback);

private:
    std::string m_label;
    std::function<void(Button *)> m_callback;
};


#endif //UI_FRAMEWORK_BUTTON_H
