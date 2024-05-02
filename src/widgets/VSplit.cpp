#include "VSplit.h"

#include <raylib.h>

#include "../Globals.h"
#include "WidgetManager.h"


VSplit::VSplit(int x, int y, int w, int h) : Widget(x, y, w, h) {
    m_follow_mouse = false;
    m_separator = .5f;

    m_top = new WidgetManager(
            AbsoluteX(),
            AbsoluteY(),
            Width(),
            (int)((float)Height()*m_separator));

    m_bottom = new WidgetManager(
            AbsoluteX(),
            AbsoluteY() + (int)((float)Height()*m_separator),
            Width(),
            Height() - (int)((float)Height()*m_separator));
}

VSplit::VSplit() : Widget(0, 0, 10, 10) {
    m_follow_mouse = false;

    m_separator = .5f;

    m_top = new WidgetManager(
            AbsoluteX(),
            AbsoluteY(),
            Width(),
            (int)((float)Height()*m_separator));

    m_bottom = new WidgetManager(
            AbsoluteX(),
            AbsoluteY() + (int)((float)Height()*m_separator),
            Width(),
            Height() - (int)((float)Height()*m_separator));

    OccupyAllSpace(0, 0, 0, 0);     //THIS MUST BE DONE AFTER INITIALISING m_left AND m_right.
}

VSplit::~VSplit() {
    delete m_top;
    delete m_bottom;
    m_top = nullptr;
    m_bottom = nullptr;
}

void VSplit::Update() {
    Widget::Update();

    bool mouse_hovering = IsMouseUsed() || !IsMouseHovering();

    HandleMovingSeparator();

    //update both widget managers even if the mouse is not hovering
    m_top->Update();
    m_bottom->Update();

    if(mouse_hovering) return;
    //TODO : splitter
    if(!IsMouseUsed()) UseMouse();
}

void VSplit::Draw() {
    //Widget::Draw();
    m_top->Draw();
    m_bottom->Draw();


    int separator_absolute_y = SeparatorAbsoluteY();
    DrawLine(AbsoluteX(), separator_absolute_y - 1,
             AbsoluteX()+Width(), separator_absolute_y - 1,
             WHITE);
    DrawLine(AbsoluteX(), separator_absolute_y + 1,
             AbsoluteX()+Width(), separator_absolute_y + 1,
             WHITE);
}

void VSplit::RecalculateAbsolutePosition() {
    Widget::RecalculateAbsolutePosition();
    RecalculateSubMangerGeometries();
}


//////////////////////////////////////////////////
// PRIVATE

void VSplit::HandleMovingSeparator() {
    if(IsMouseButtonUp(MOUSE_BUTTON_LEFT)) {
        m_follow_mouse = false;
    }

    if(m_follow_mouse) {
        int relative_mouse_y = GetMouseY() - AbsoluteY();
        float new_percentage = (float)relative_mouse_y / (float)Height();
        m_separator = new_percentage;
        if(m_separator < .01f) m_separator = .01f;
        if(m_separator > .99f) m_separator = .99f;
        RecalculateSubMangerGeometries();
    }

    if(IsMouseUsed() || !IsMouseHovering()) return;
    if(!IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) return;

    int mouse_y = GetMouseY();
    int separator_absolute_y = SeparatorAbsoluteY();

    if(mouse_y > separator_absolute_y-4 && mouse_y < separator_absolute_y+4) {
        m_follow_mouse = true;
        UseMouse();
    }
}

void VSplit::RecalculateSubMangerGeometries() {
    if(m_top != nullptr) {
        m_top->SetPosition(AbsoluteX(), AbsoluteY());
        m_top->SetSize(Width(), (int)((float)Height()*m_separator));
    }

    if(m_bottom != nullptr) {
        m_bottom->SetPosition(AbsoluteX(), AbsoluteY() + (int)((float)Height()*m_separator));
        m_bottom->SetSize(Width(), Height() - (int)((float)Height()*m_separator));
    }
}

int VSplit::SeparatorAbsoluteY() {
    return AbsoluteY() + (int)((float)Height()*m_separator);
}
