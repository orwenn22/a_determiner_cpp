#ifndef UI_FRAMEWORK_WINDOW_H
#define UI_FRAMEWORK_WINDOW_H

#include <raylib.h>
#include <string>

class WindowManager;
class WidgetManager;

class Window {
public:
    Window(int x, int y, int w, int h);
    virtual ~Window();

    void Update();
    void Draw();

    //Getters
    inline int X() { return m_x; }
    inline int Y() { return m_y; }
    inline int Width() { return m_w; }
    inline int Height() { return m_h; }
    inline WidgetManager *GetWidgetManager() { return m_widgets; }

    //Setters
    void SetPosition(int x, int y);
    void SetSize(int w, int h);
    void SetTitle(std::string title);

    bool IsMouseHovering();


protected:
    //Customisation
    Color m_background_color;
    Color m_titlebar_color;
    Color m_title_color;

private:
    friend WindowManager;

    //Also handle bringing the window on top & closing
    void HandleDrag();

    int m_x, m_y, m_w, m_h;     //The position is absolute
    std::string m_title;
    WindowManager *m_manager;
    WidgetManager *m_widgets;

    //used for dragging the window
    bool m_follow_mouse;
    int m_follow_offset_x;
    int m_follow_offset_y;
};


#endif //UI_FRAMEWORK_WINDOW_H
