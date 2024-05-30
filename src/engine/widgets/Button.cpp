#include "Button.h"

#include <iostream>
#include "raylib.h"

#include "../Globals.h"


void DefaultButtonCallback(Button *button) {
    std::cout << "DefaultButtonCallback : button clicked !\n";
}

Button::Button(int x, int y, int w, int h, std::string label, std::function<void(void)> callback) : Clickable(x, y, w, h, callback) {
    m_label = label;
    m_font_size = 10;
    m_hover_offset = 2;
    m_label_inner_x = 2;
    m_label_inner_y = 2;
    m_color = {255, 255, 255, 255};
    m_hover_color = YELLOW;
}

void Button::Draw() {
    int x = AbsoluteX();
    int y = AbsoluteY() + m_hovered*m_hover_offset;
    DrawRectangle(x, y, Width(), Height(), (m_hovered) ? m_hover_color : m_color);
    DrawRectangle(x+1, y+1, Width()-2, Height()-2, BLACK);
    DrawText(m_label.c_str(), x+m_label_inner_x, y+m_label_inner_y, m_font_size, WHITE);
}

void Button::SetLabel(std::string label) {
    m_label = label;
}

void Button::SetLabelPosition(int x, int y) {
    m_label_inner_x = x;
    m_label_inner_y = y;
}

void Button::CenterLabel() {
    m_label_inner_x = (Width() - MeasureText(m_label.c_str(), m_font_size)) / 2;
    m_label_inner_y = (Height() - m_font_size) / 2;
}

void Button::SetFontSize(int font_size) {
    if(font_size < 1) return;
    m_font_size = font_size;
}
