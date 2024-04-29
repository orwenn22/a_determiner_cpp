#include "Button.h"

#include <iostream>
#include <raylib.h>

#include "../Globals.h"


void DefaultButtonCallback(Button *button) {
    std::cout << "DefaultButtonCallback : button clicked !\n";
}

Button::Button(int x, int y, int w, int h, std::string label, std::function<void(Button *)> callback) : Widget(x, y, w, h) {
    m_label = label;
    m_callback = callback;
}

void Button::Update() {
    if(IsMouseUsed() || !IsMouseHovering()) return;

    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        m_callback(this);
    }

    UseMouse();
}

void Button::Draw() {
    DrawRectangle(AbsoluteX(), AbsoluteY(), Width(), Height(), WHITE);
    DrawRectangle(AbsoluteX()+1, AbsoluteY()+1, Width()-2, Height()-2, BLACK);
    DrawText(m_label.c_str(), AbsoluteX()+2, AbsoluteY()+2, 10, WHITE);
}

void Button::SetLabel(std::string label) {
    m_label = label;
}

void Button::SetCallback(std::function<void(Button *)> callback) {
    m_callback = callback;
}