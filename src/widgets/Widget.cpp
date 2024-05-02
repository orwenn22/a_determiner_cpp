#include "Widget.h"

#include <raylib.h>

#include "WidgetManager.h"


Widget::Widget(int x, int y, int w, int h) {
    m_manager = nullptr;
    m_relative_x = x;
    m_relative_y = y;
    m_w = w;
    m_h = h;
    m_alignment = WidgetAlignment_None;

    m_occupy_all_space = false;
    m_full_size_offset_right = 0;
    m_full_size_offset_bottom = 0;
}

Widget::~Widget() = default;

void Widget::Update() {
    //Must be defined by subclasses
}

void Widget::Draw() {
    DrawRectangle(m_absolute_x, m_absolute_y, m_w, m_h, RED);
}

void Widget::RecalculateAbsolutePosition() {
    if(m_occupy_all_space) RecalculateAbsolutePosOccupyAll();
    else RecalculateAbsolutePosDefault();
}

bool Widget::IsMouseHovering() {
    int mouse_x = GetMouseX();
    int mouse_y = GetMouseY();
    return (mouse_x >= m_absolute_x && mouse_y >= m_absolute_y && mouse_x < m_absolute_x+m_w && mouse_y < m_absolute_y+m_h);
}

void Widget::SetX(int x) {
    m_relative_x = x;
    m_occupy_all_space = false;
    RecalculateAbsolutePosition();
}
void Widget::SetY(int y) {
    m_relative_y = y;
    m_occupy_all_space = false;
    RecalculateAbsolutePosition();
}
void Widget::SetPosition(int x, int y) {
    m_relative_x = x;
    m_relative_y = y;
    m_occupy_all_space = false;
    RecalculateAbsolutePosition();
}

void Widget::SetWidth(int w) {
    m_w = w;
    m_occupy_all_space = false;
    RecalculateAbsolutePosition();
}
void Widget::SetHeight(int h) {
    m_h = h;
    m_occupy_all_space = false;
    RecalculateAbsolutePosition();
}
void Widget::SetSize(int w, int h) {
    m_w = w;
    m_h = h;
    m_occupy_all_space = false;
    RecalculateAbsolutePosition();
}

void Widget::SetAlignment(WidgetAlignment alignment) {
    m_alignment = alignment;
    m_occupy_all_space = false;
    RecalculateAbsolutePosition();
}

void Widget::OccupyAllSpace(int top, int left, int right, int bottom) {
    m_occupy_all_space = true;
    m_alignment = WidgetAlignment_None;
    m_relative_x = left;
    m_relative_y = top;
    m_full_size_offset_right = right;
    m_full_size_offset_bottom = bottom;
    RecalculateAbsolutePosition();
}


/////////////////////////////////////////////////////////////////////////
// PRIVATE

void Widget::SetManager(WidgetManager *manager) {
    m_manager = manager;
    RecalculateAbsolutePosition();
}

void Widget::RecalculateAbsolutePosDefault() {
    int container_x, container_y, container_w, container_h;
    if(m_manager == nullptr) {
        container_x = 0;
        container_y = 0;
        container_w = GetScreenWidth();
        container_h = GetScreenHeight();
    }
    else {
        container_x = m_manager->X();
        container_y = m_manager->Y();
        container_w = m_manager->Width();
        container_h = m_manager->Height();
    }

    switch(m_alignment & 0x0f) {    //horizontal
        case WidgetAlignment_None:
            m_absolute_x = container_x + m_relative_x;
            break;

        case WidgetAlignment_HCenter:
            m_absolute_x = container_x + container_w/2 - m_w/2 + m_relative_x;
            break;

        case WidgetAlignment_Right:
            m_absolute_x = container_x + container_w - m_w - m_relative_x;
            break;

        default:
            //wat
            break;
    }

    switch(m_alignment & 0xf0) {    //vertical
        case WidgetAlignment_None:
            m_absolute_y = container_y + m_relative_y;
            break;

        case WidgetAlignment_VCenter:
            m_absolute_y = container_y + container_h/2 - m_h/2 + m_relative_y;
            break;

        case WidgetAlignment_Bottom:
            m_absolute_y = container_y + container_h - m_h - m_relative_y;
            break;

        default:
            //wat
            break;
    }
}

void Widget::RecalculateAbsolutePosOccupyAll() {
    int container_x, container_y, container_w, container_h;
    if(m_manager == nullptr) {
        container_x = 0;
        container_y = 0;
        container_w = GetScreenWidth();
        container_h = GetScreenHeight();
    }
    else {
        container_x = m_manager->X();
        container_y = m_manager->Y();
        container_w = m_manager->Width();
        container_h = m_manager->Height();
    }

    m_absolute_x = container_x + m_relative_x;
    m_absolute_y = container_y + m_relative_y;
    m_w = container_w - m_relative_x - m_full_size_offset_right;
    m_h = container_h - m_relative_y - m_full_size_offset_bottom;
}
