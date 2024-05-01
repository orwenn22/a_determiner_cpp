#include "Clickable.h"

#include <iostream>
#include <raylib.h>

#include "../Globals.h"


void DefaultClickableCallback() {
    std::cout << "DefaultClickableCallback : button clicked !\n";
}


Clickable::Clickable(int x, int y, int w, int h, std::function<void(void)> callback) : Widget(x, y, w, h) {
    m_callback = callback;
    m_hovered = false;
}


void Clickable::Update() {
    m_hovered = false;
    if(IsMouseUsed() || !IsMouseHovering()) return;

    m_hovered = true;
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        m_callback();
    }

    UseMouse();
}

void Clickable::SetCallback(std::function<void(void)> callback) {
    m_callback = callback;
}

