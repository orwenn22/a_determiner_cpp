#ifndef UI_FRAMEWORK_WINDOWMANAGER_H
#define UI_FRAMEWORK_WINDOWMANAGER_H

#include <vector>


class Window;

class WindowManager {
public:
    WindowManager();
    WindowManager(int x, int y, int w, int h);
    ~WindowManager();

    void Update();
    void Draw();

    //Getters
    inline int X() { return m_x; }
    inline int Y() { return m_y; }
    inline int Width() { return m_w; }
    inline int Height() { return m_h; }

    //Setters
    void SetPosition(int x, int y);
    void SetSize(int w, int h);
    void MakeFullscreen();

    void AddWindow(Window *window);
    void RemoveWindowByPtr(Window *window);     //Won"t deallocate
    void CloseWindowByPtr(Window *window);      //Will deallocate
    void BringOnTop(Window *window);
    void Clear();

private:
    void MakeWindowsInBound();
    void FreeTrashcan();

    std::vector<Window *> m_windows;
    std::vector<Window *> m_trashcan;
    bool m_fullscreen;
    int m_x, m_y, m_w, m_h;
};


#endif //UI_FRAMEWORK_WINDOWMANAGER_H
