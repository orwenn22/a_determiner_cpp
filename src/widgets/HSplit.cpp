#include "HSplit.h"

#include <raylib.h>

#include "../Globals.h"
#include "WidgetManager.h"


HSplit::HSplit(int x, int y, int w, int h) : Widget(x, y, w, h) {
    m_follow_mouse = false;
    m_occupy_all_space = false;
    m_full_size_offset_top = 0;
    m_full_size_offset_left = 0;
    m_full_size_offset_right = 0;
    m_full_size_offset_bottom = 0;

    m_separator = .5f;

    m_left = new WidgetManager(
            AbsoluteX(),
            AbsoluteY(),
            (int)((float)Width()*m_separator),
            Height());

    m_right = new WidgetManager(
            AbsoluteX() + (int)((float)Width()*m_separator),
            AbsoluteY(),
            Width() - (int)((float)Width() * (1.f-m_separator)),
            Height());
}

HSplit::HSplit() : Widget(0, 0, 10, 10) {
    m_follow_mouse = false;
    m_occupy_all_space = true;
    m_full_size_offset_top = 20;
    m_full_size_offset_left = 20;
    m_full_size_offset_right = 20;
    m_full_size_offset_bottom = 20;

    m_separator = .5f;

    m_left = new WidgetManager(
            AbsoluteX(),
            AbsoluteY(),
            (int)((float)Width()*m_separator),
            Height());

    m_right = new WidgetManager(
            AbsoluteX() + (int)((float)Width()*m_separator),
            AbsoluteY(),
            Width() - (int)((float)Width() * (1.f-m_separator)),
            Height());
}

HSplit::~HSplit() {
    delete m_left;
    delete m_right;
    m_left = nullptr;
    m_right = nullptr;
}


void HSplit::Update() {
    Widget::Update();

    bool mouse_hovering = IsMouseUsed() || !IsMouseHovering();

    HandleMovingSeparator();

    //update both widget managers even if the mouse is not hovering
    m_left->Update();
    m_right->Update();

    if(mouse_hovering) return;
    //TODO : splitter
    if(!IsMouseUsed()) UseMouse();
}


void HSplit::Draw() {
    //Widget::Draw();
    m_left->Draw();
    m_right->Draw();


    int separator_absolute_x = SeparatorAbsoluteX();
    DrawLine(separator_absolute_x - 1, AbsoluteY(),
             separator_absolute_x - 1, AbsoluteY()+Height(),
            WHITE);
    DrawLine(separator_absolute_x + 1, AbsoluteY(),
             separator_absolute_x + 1, AbsoluteY()+Height(),
            WHITE);
}


void HSplit::RecalculateAbsolutePosition() {
    if(!m_occupy_all_space) {
        Widget::RecalculateAbsolutePosition();
        RecalculateSubMangerGeometries();
        return;
    }

    WidgetManager *manager = GetManager();
    int container_x, container_y, container_w, container_h;
    if(manager == nullptr) {
        container_x = 0;
        container_y = 0;
        container_w = GetScreenWidth();
        container_h = GetScreenHeight();
    }
    else {
        container_x = manager->X();
        container_y = manager->Y();
        container_w = manager->Width();
        container_h = manager->Height();
    }

    m_absolute_x = container_x + m_full_size_offset_left;
    m_absolute_y = container_y + m_full_size_offset_top;
    m_w = container_w - m_full_size_offset_left - m_full_size_offset_right;
    m_h = container_h - m_full_size_offset_top - m_full_size_offset_bottom;

    RecalculateSubMangerGeometries();
}


/////////////////////////
// PRIVATE

void HSplit::HandleMovingSeparator() {
    if(IsMouseButtonUp(MOUSE_BUTTON_LEFT)) {
        m_follow_mouse = false;
    }

    if(m_follow_mouse) {
        int relative_mouse_x = GetMouseX() - AbsoluteX();
        float new_percentage = (float)relative_mouse_x / (float)Width();
        m_separator = new_percentage;
        if(m_separator < .01f) m_separator = .01f;
        if(m_separator > .99f) m_separator = .99f;
        RecalculateSubMangerGeometries();
    }

    if(IsMouseUsed() || !IsMouseHovering()) return;
    if(!IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) return;

    int mouse_x = GetMouseX();
    int separator_absolute_x = SeparatorAbsoluteX();

    if(mouse_x > separator_absolute_x-4 && mouse_x < separator_absolute_x+4) {
        m_follow_mouse = true;
        UseMouse();
    }
}

void HSplit::RecalculateSubMangerGeometries() {
    if(m_left != nullptr) {
        m_left->SetPosition(AbsoluteX(), AbsoluteY());
        m_left->SetSize((int) ((float) Width() * m_separator), Height());
    }

    if(m_right != nullptr) {
        m_right->SetPosition(AbsoluteX() + (int) ((float) Width() * m_separator), AbsoluteY());
        m_right->SetSize(Width() - (int) ((float) Width() * (1.f - m_separator)), Height());
    }
}

int HSplit::SeparatorAbsoluteX() {
    return AbsoluteX() + (int)((float)Width()*m_separator);
}
