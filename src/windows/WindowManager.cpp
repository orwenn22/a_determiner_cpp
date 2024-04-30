#include "WindowManager.h"

#include <algorithm>
#include <raylib.h>

#include "Window.h"


WindowManager::WindowManager() {
    m_fullscreen = true;
    m_x = 0;
    m_y = 0;
    m_w = GetScreenWidth();
    m_h = GetScreenHeight();
}

WindowManager::WindowManager(int x, int y, int w, int h) {
    m_fullscreen = false;
    m_x = x;
    m_y = y;
    m_w = w;
    m_h = h;
}

WindowManager::~WindowManager() {
    for(Window *w : m_windows) {
        delete w;
    }
    m_windows.clear();
    FreeTrashcan();
}

void WindowManager::Update() {
    FreeTrashcan();

    if(m_fullscreen) {
        int sw = GetScreenWidth();
        int sh = GetScreenHeight();
        if(m_w != sw || m_h != sh) {
            m_w = sw;
            m_h = sh;
            MakeWindowsInBound();
        }
    }

    for(int i = 0; i < m_windows.size(); ++i) {     //Don't use iterator here in case a window self-destruct itself
        m_windows[i]->Update();
    }
}

void WindowManager::Draw() {
    for(auto it = m_windows.rbegin(); it != m_windows.rend(); ++it) {
        (*it)->Draw();
    }
}

void WindowManager::SetPosition(int x, int y) {
    m_fullscreen = false;

    //Recalculate the absolute position of each window
    int offset_x = x - m_x;
    int offset_y = y - m_y;
    for(Window *w : m_windows) {
        w->m_x += offset_x;
        w->m_y += offset_y;
    }

    m_x = x;
    m_y = y;
}

void WindowManager::SetSize(int w, int h) {
    m_fullscreen = false;
    m_w = w;
    m_h = h;
    MakeWindowsInBound();
}

void WindowManager::MakeFullscreen() {
    m_fullscreen = true;
    m_x = 0;
    m_y = 0;
    m_w = GetScreenWidth();
    m_y = GetScreenHeight();
}


void WindowManager::AddWindow(Window *window) {
    if(window == nullptr) return;
    if(window->m_manager != nullptr) return;
    m_windows.push_back(window);
    window->m_manager = this;
}

void WindowManager::RemoveWindowByPtr(Window *window) {
    if(window == nullptr) return;

    auto result = std::find(m_windows.begin(), m_windows.end(), window);
    if(result == m_windows.end()) return;
    m_windows.erase(result);
    window->m_manager = nullptr;
}

void WindowManager::CloseWindowByPtr(Window *window) {
    if(window == nullptr) return;

    auto result = std::find(m_windows.begin(), m_windows.end(), window);
    if(result == m_windows.end()) return;
    m_windows.erase(result);
    window->m_manager = nullptr;
    m_trashcan.push_back(window);
}

void WindowManager::BringOnTop(Window *window) {
    if(window == nullptr) return;

    auto result = std::find(m_windows.begin(), m_windows.end(), window);
    if(result == m_windows.end()) return;

    m_windows.erase(result);
    m_windows.insert(m_windows.begin(), window);
}

void WindowManager::MakeWindowsInBound() {
    for(Window *win : m_windows) {
        if(win->X() + win->Width() >= m_x + m_w) win->SetPosition(m_x + m_w - win->Width() - 1, win->Y());
        if(win->Y() + win->Height() >= m_y + m_h) win->SetPosition(win->X(), m_y + m_h - win->Height() - 1);
        if(win->X() < m_x) win->SetPosition(m_x, win->Y());
        if(win->Y() < m_y) win->SetPosition(win->X(), m_y);
    }
}

void WindowManager::FreeTrashcan() {
    for(Window *w : m_trashcan) delete w;
    m_trashcan.clear();
}
