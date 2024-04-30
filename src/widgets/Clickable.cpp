#include "Clickable.h"

#include <iostream>
#include <raylib.h>

#include "../Globals.h"

Clickable::Clickable(int x, int y, int w, int h, std::function<void(Clickable *)> callback) : Widget(x, y, w, h) {
    m_callback = callback;
    m_hovered = false;
}


void DefaultClickableCallback(Clickable *clickable) {
    std::cout << "DefaultClickableCallback : button clicked !\n";
}

void Clickable::Update() {
    m_hovered = false;
    if(IsMouseUsed() || !IsMouseHovering()) return;

    m_hovered = true;
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        m_callback(this);
    }

    UseMouse();
}

void Clickable::SetCallback(std::function<void(Clickable *)> callback) {
    m_callback = callback;
}

