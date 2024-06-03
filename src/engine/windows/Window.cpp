#include "Window.h"

#include "raylib.h"

#include "WindowManager.h"
#include "../Globals.h"
#include "../util/Trace.h"
#include "../widgets/WidgetManager.h"

#define TITLEBAR_HEIGHT 12
#define CLOSE_WIDTH TITLEBAR_HEIGHT


Window::Window(int x, int y, int w, int h) {
    m_x = x;
    m_y = y;
    m_w = w;
    m_h = h;
    m_title = "window";
    m_manager = nullptr;
    m_widgets = new WidgetManager(x, y + TITLEBAR_HEIGHT, w, h - TITLEBAR_HEIGHT);

    m_follow_mouse = false;
    m_follow_offset_x = 0;
    m_follow_offset_y = 0;

    m_background_color = {0x11, 0x11, 0x11, 0xff};
    m_titlebar_color = BLUE;
    m_title_color = WHITE;
}

Window::~Window() {
    delete m_widgets;
}

void Window::Update() {
    HandleDrag();       //NOTE : this does not use the mouse
    m_widgets->Update();        //Update widgets even if the mouse is outside the window

    if(!IsMouseHovering()) return;
    //If we get here then the window is hovered
    if(!IsMouseUsed()) UseMouse();      //Make sure the click can't pass through the window
}

void Window::Draw() {
    DrawRectangle(m_x, m_y, m_w, m_h, m_background_color);        //bg
    DrawRectangle(m_x, m_y, m_w, TITLEBAR_HEIGHT, m_titlebar_color);        //title bar
    DrawText(m_title.c_str(), m_x+2, m_y+2, 10, m_title_color);             //title
    DrawRectangle(m_x+m_w-CLOSE_WIDTH, m_y, CLOSE_WIDTH, TITLEBAR_HEIGHT, RED);     //close button
    m_widgets->Draw();
    DrawRectangleLines(m_x, m_y, m_w, m_h, WHITE);      //outline
}

void Window::SetPosition(int x, int y) {
    int contaiter_x, container_y, container_w, container_h;
    if(m_manager == nullptr) {
        contaiter_x = 0;
        container_y = 0;
        container_w = GetScreenWidth();
        container_h = GetScreenHeight();
    }
    else {
        contaiter_x = m_manager->X();
        container_y = m_manager->Y();
        container_w = m_manager->Width();
        container_h = m_manager->Height();
    }
    
    if(x+m_w >= contaiter_x+container_w) x = contaiter_x+container_w - m_w - 1;
    if(y+m_h >= container_y+container_h) y = container_y+container_h - m_h - 1;
    if(x < contaiter_x) x = contaiter_x;
    if(y < container_y) y = container_y;
    
    m_x = x;
    m_y = y;
    m_widgets->SetPosition(m_x, m_y + TITLEBAR_HEIGHT);
}

void Window::SetSize(int w, int h) {
    if(w < 1 || h < 1) return;

    m_w = w;
    m_h = h;
    m_widgets->SetSize(m_w, m_h-TITLEBAR_HEIGHT);
}

void Window::SetTitle(std::string title) {
    m_title = title;
}


bool Window::IsMouseHovering() {
    int mouse_x = GetMouseX();
    int mouse_y = GetMouseY();
    return (mouse_x >= m_x && mouse_x < m_x+m_w && mouse_y >= m_y && mouse_y < m_y+m_h);
}


//////////////////////////////
// PRIVATE

void Window::HandleDrag() {
    if(m_follow_mouse) {
        SetPosition(GetMouseX() - m_follow_offset_x, GetMouseY() - m_follow_offset_y);
    }
    if(IsMouseButtonUp(MOUSE_BUTTON_LEFT)) {
        m_follow_mouse = false;
    }

    if(IsMouseUsed() || !IsMouseHovering()) return;     //Check if the mouse is on the window

    int relative_mouse_x = GetMouseX() - m_x;
    int relative_mouse_y = GetMouseY() - m_y;

    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && relative_mouse_y < TITLEBAR_HEIGHT) {
        if(relative_mouse_x < m_w - CLOSE_WIDTH) {      //On title bar (not close button)
            m_follow_mouse = true;
            m_follow_offset_x = relative_mouse_x;
            m_follow_offset_y = relative_mouse_y;
        }
        else {      //On close button
            if(m_manager == nullptr) {
                TRACE("Can't close a window that is not in a manager\n");
                return;
            }
            m_manager->CloseWindowByPtr(this);
        }
    }

    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && m_manager != nullptr) {
        m_manager->BringOnTop(this);
    }
}
