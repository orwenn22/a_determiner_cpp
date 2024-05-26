#include "Label.h"

#include "raylib.h"


Label::Label(int x, int y, int font_size, std::string label) : Widget(x, y, 1, font_size) {
    m_font_size = font_size;
    SetLabel(label);
    m_color = WHITE;
}

void Label::Draw() {
    //Widget::Draw();
    DrawText(m_label.c_str(), AbsoluteX(), AbsoluteY(), m_font_size, m_color);
}

void Label::SetLabel(std::string label) {
    m_label = label;
    SetWidth(MeasureText(m_label.c_str(), m_font_size));
}

void Label::SetFontSize(int font_size) {
    m_font_size = font_size;
    SetWidth(MeasureText(m_label.c_str(), m_font_size));
}

void Label::SetColor(Color color) {
    m_color = color;
}
