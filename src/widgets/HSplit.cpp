#include "HSplit.h"

#include <raylib.h>

#include "../Globals.h"
#include "WidgetManager.h"


HSplit::HSplit(int x, int y, int w, int h) : Widget(x, y, w, h) {
    m_follow_mouse = false;
    m_separator = .5f;

    m_left = new WidgetManager(
            AbsoluteX(),
            AbsoluteY(),
            (int)((float)Width()*m_separator),
            Height());

    m_right = new WidgetManager(
            AbsoluteX() + (int)((float)Width()*m_separator),
            AbsoluteY(),
            Width() - (int) ((float)Width() * m_separator),
            Height());
}

HSplit::HSplit() : Widget(0, 0, 10, 10) {
    m_follow_mouse = false;

    m_separator = .5f;

    m_left = new WidgetManager(
            AbsoluteX(),
            AbsoluteY(),
            (int)((float)Width()*m_separator),
            Height());

    m_right = new WidgetManager(
            AbsoluteX() + (int)((float)Width()*m_separator),
            AbsoluteY(),
            Width() - (int) ((float)Width() * m_separator),
            Height());

    OccupyAllSpace(0, 0, 0, 0);     //THIS MUST BE DONE AFTER INITIALISING m_left AND m_right.
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
    Widget::RecalculateAbsolutePosition();
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
        m_right->SetSize(Width() - (int) ((float)Width() * m_separator), Height());
    }
}

int HSplit::SeparatorAbsoluteX() {
    return AbsoluteX() + (int)((float)Width()*m_separator);
}
