#include "Label.h"

#include "raylib.h"


Label::Label(int x, int y, int font_size, std::string label) : Widget(x, y, 1, font_size) {
    m_font_size = font_size;
    SetLabel(label);
    m_color = WHITE;
    m_enable_outline = false;
    m_outline_color = BLACK;
}

void Label::Draw() {
    const char *cstr = m_label.c_str();     //DO NOT EDIT THIS

    if(m_enable_outline) {
        int offset = m_font_size/10;
        DrawText(cstr, AbsoluteX()+offset, AbsoluteY()+offset, m_font_size, m_outline_color);
        DrawText(cstr, AbsoluteX()+offset, AbsoluteY(), m_font_size, m_outline_color);
        DrawText(cstr, AbsoluteX()+offset, AbsoluteY()-offset, m_font_size, m_outline_color);

        DrawText(cstr, AbsoluteX(), AbsoluteY()+offset, m_font_size, m_outline_color);
        DrawText(cstr, AbsoluteX(), AbsoluteY()-offset, m_font_size, m_outline_color);

        DrawText(cstr, AbsoluteX()-offset, AbsoluteY()+offset, m_font_size, m_outline_color);
        DrawText(cstr, AbsoluteX()-offset, AbsoluteY(), m_font_size, m_outline_color);
        DrawText(cstr, AbsoluteX()-offset, AbsoluteY()-offset, m_font_size, m_outline_color);
    }

    DrawText(cstr, AbsoluteX(), AbsoluteY(), m_font_size, m_color);
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

void Label::SetOutline(bool enabled) {
    m_enable_outline = enabled;
}

void Label::SetOutlineColor(Color outline_color) {
    m_outline_color = outline_color;
}
