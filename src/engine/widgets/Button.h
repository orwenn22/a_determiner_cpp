#ifndef UI_FRAMEWORK_BUTTON_H
#define UI_FRAMEWORK_BUTTON_H

#include "Clickable.h"

#include <functional>
#include "raylib.h"
#include <string>

class Button;



class Button : public Clickable {
public:
    Button(int x, int y, int w, int h, std::string label = "button", std::function<void(void)> callback = DefaultClickableCallback);

    void Draw() override;

    void SetLabel(std::string label);

private:
    std::string m_label;
    int m_hover_offset;
};


#endif //UI_FRAMEWORK_BUTTON_H
