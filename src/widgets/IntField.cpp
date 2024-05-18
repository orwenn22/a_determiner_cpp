#include "IntField.h"

#include <raylib.h>

#include "../Globals.h"


IntField::IntField(int x, int y, int w, int font_size, int *target) : Widget(x, y, w, font_size + 4) {
    m_target = target;
    m_displayed_value = (m_target != nullptr) ? (*m_target) : 0;
    m_selected = false;
    m_font_size = font_size;
}

IntField::~IntField() = default;


void IntField::Update() {
    Widget::Update();

    ProcessKeyboardInput();

    if(IsMouseUsed() || !IsMouseHovering()) {
        //Clicking anywhere else will deselect the widget
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            m_selected = false;
            if(m_target != nullptr) m_displayed_value = (*m_target);
        }
        return;
    }

    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) m_selected = true;
    UseMouse();
}

void IntField::Draw() {
    //Widget::Draw();

    DrawRectangle(AbsoluteX(), AbsoluteY(), Width(), Height(), {0x14, 0x14, 0x14, 0xff});
    DrawRectangleLines(AbsoluteX(), AbsoluteY(), Width(), Height(), m_selected ? YELLOW : WHITE);

    DrawText(TextFormat("%i", m_displayed_value), AbsoluteX()+2, AbsoluteY()+2, m_font_size, WHITE);
}

void IntField::SetMin(int value) {
    m_min = value;
}

void IntField::SetMax(int value) {
    m_max = value;
}


///////////////////////////////////////
//// PRIVATE

void IntField::ProcessKeyboardInput() {
    if(!m_selected || m_target == nullptr) return;

    bool need_update = false;

    for(int i = KEY_ZERO; i <= KEY_NINE; ++i) {
        if(IsKeyPressed((KeyboardKey)i)) {
            int digit = i - KEY_ZERO;
            m_displayed_value *= 10;
            m_displayed_value += digit;
            need_update = true;
        }
    }

    for(int i = KEY_KP_0; i <= KEY_KP_9; ++i) {
        if(IsKeyPressed((KeyboardKey)i)) {
            int digit = i - KEY_KP_0;
            m_displayed_value *= 10;
            m_displayed_value += digit;
            need_update = true;
        }
    }

    if(IsKeyPressed(KEY_MINUS) || IsKeyPressed(KEY_KP_SUBTRACT)) {
        m_displayed_value = -m_displayed_value;
        need_update = true;
    }

    if(IsKeyPressed(KEY_BACKSPACE)) {
        m_displayed_value /= 10;
        need_update = true;
    }

    if(need_update) {
        if(m_min.has_value() && m_displayed_value < m_min.value()) {
            (*m_target) = m_min.value();
        }
        else if(m_max.has_value() && m_displayed_value > m_max.value()) {
            (*m_target) = m_max.value();
        }
        else {
            (*m_target) = m_displayed_value;
        }
    }
}
