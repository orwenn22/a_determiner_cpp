#include "Button.h"

#include <iostream>
#include <raylib.h>

#include "../Globals.h"


void DefaultButtonCallback(Button *button) {
    std::cout << "DefaultButtonCallback : button clicked !\n";
}

Button::Button(int x, int y, int w, int h, std::string label, std::function<void(Clickable *)> callback) : Clickable(x, y, w, h, callback) {
    m_label = label;
    m_hover_offset = 2;
}

void Button::Draw() {
    int x = AbsoluteX();
    int y = AbsoluteY() + m_hovered*m_hover_offset;
    DrawRectangle(x, y, Width(), Height(), WHITE);
    DrawRectangle(x+1, y+1, Width()-2, Height()-2, BLACK);
    DrawText(m_label.c_str(), x+2, y+2, 10, WHITE);
}

void Button::SetLabel(std::string label) {
    m_label = label;
}
