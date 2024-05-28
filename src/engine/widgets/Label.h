#ifndef UI_FRAMEWORK_LABEL_H
#define UI_FRAMEWORK_LABEL_H

#include "Widget.h"

#include "raylib.h"
#include <string>


//TODO : custom font
class Label : public Widget {
public:
    Label(int x, int y, int font_size, std::string label);

    void Draw() override;

    void SetLabel(std::string label);
    void SetFontSize(int font_size);
    void SetColor(Color color);
    void SetOutline(bool enabled);
    void SetOutlineColor(Color outline_color);

private:
    std::string m_label;
    int m_font_size;
    Color m_color;
    bool m_enable_outline;
    Color m_outline_color;
};


#endif //UI_FRAMEWORK_LABEL_H
