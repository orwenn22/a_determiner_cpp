#ifndef UI_FRAMEWORK_WIDGETMANAGER_H
#define UI_FRAMEWORK_WIDGETMANAGER_H

#include <vector>

class Widget;

class WidgetManager {
public:
    WidgetManager();
    WidgetManager(int x, int y, int w, int h);
    ~WidgetManager();

    void Update();
    void Draw();

    void AddWidget(Widget *w);
    void RemoveWidget(Widget *w);

    //Getters
    inline int X() { return m_x; }
    inline int Y() { return m_y; }
    inline int Width() { return m_w; }
    inline int Height() { return m_h; }

    void SetPosition(int x, int y);
    void SetSize(int w, int h);
    void MakeFullscreen();

private:
    void RecalculateWidgetPositions();

    bool m_fullscreen;
    int m_x, m_y, m_w, m_h;
    std::vector<Widget *> m_widgets;
};


#endif //UI_FRAMEWORK_WIDGETMANAGER_H
