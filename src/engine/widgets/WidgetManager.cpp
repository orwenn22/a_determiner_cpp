#include "WidgetManager.h"

#include <algorithm>
#include "raylib.h"
#include <iostream>

#include "../util/Scissors.h"
#include "Widget.h"

WidgetManager::WidgetManager() {
    m_x = 0;
    m_y = 0;
    m_w = GetScreenWidth();
    m_h = GetScreenHeight();
    m_fullscreen = true;
    std::cout << "WidgetManager::WidgetManager : " << m_x << " " << m_y << " " << m_w << " " << m_h << "\n";
}

WidgetManager::WidgetManager(int x, int y, int w, int h) {
    m_x = x;
    m_y = y;
    m_w = w;
    m_h = h;
    m_fullscreen = false;
    std::cout << "WidgetManager::WidgetManager : " << m_x << " " << m_y << " " << m_w << " " << m_h << "\n";
}

WidgetManager::~WidgetManager() {
    Clear();
}

void WidgetManager::Update() {
    if(m_fullscreen) {
        int screen_w = GetScreenWidth();
        int screen_h = GetScreenHeight();
        if(screen_w != m_w || screen_h != m_h) {
            m_w = screen_w;
            m_h = screen_h;
            RecalculateWidgetPositions();
        }
    }

    //Despite what clion is telling, we should NOT use range based loop here, in the case a widget self-destruct itself
    for(size_t i = 0; i < m_widgets.size(); ++i) m_widgets[i]->Update();
}

void WidgetManager::Draw() {
    PushScissors({(float)m_x, (float)m_y, (float)m_w, (float)m_h});
    for(Widget *w : m_widgets) w->Draw();
    PopScissors();
}

void WidgetManager::AddWidget(Widget *w) {
    if(w == nullptr) return;
    if(w->m_manager != nullptr) return;

    m_widgets.push_back(w);
    w->SetManager(this);
}

void WidgetManager::RemoveWidget(Widget *w) {
    if(w == nullptr) return;
    auto result = std::find(m_widgets.begin(), m_widgets.end(), w);
    if(result == m_widgets.end()) return;
    m_widgets.erase(result);
    w->SetManager(nullptr);
}

void WidgetManager::Clear() {
    for(Widget *w : m_widgets) {
        w->m_manager = nullptr;     //Do it this way in order to not recalculate the absolute position
        delete w;
    }
    m_widgets.clear();
}

void WidgetManager::SetPosition(int x, int y) {
    m_x = x;
    m_y = y;
    m_fullscreen = false;
    RecalculateWidgetPositions();
}
void WidgetManager::SetSize(int w, int h) {
    m_w = w;
    m_h = h;
    m_fullscreen = false;
    RecalculateWidgetPositions();
}

void WidgetManager::MakeFullscreen() {
    m_fullscreen = true;
    m_x = 0;
    m_y = 0;
    m_w = GetScreenWidth();
    m_h = GetScreenHeight();
    RecalculateWidgetPositions();
}


void WidgetManager::RecalculateWidgetPositions() {
    for(Widget *w : m_widgets) w->RecalculateAbsolutePosition();
}