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
    void SetLabelPosition(int x, int y);
    void CenterLabel();
    void SetFontSize(int font_size);

protected:
    std::string m_label;
    int m_font_size;
    int m_hover_offset;
    int m_label_inner_x;
    int m_label_inner_y;
    Color m_color;
    Color m_hover_color;
};


#endif //UI_FRAMEWORK_BUTTON_H
