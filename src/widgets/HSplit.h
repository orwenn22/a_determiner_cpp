#ifndef UI_FRAMEWORK_HSPLIT_H
#define UI_FRAMEWORK_HSPLIT_H

#include "Widget.h"

class HSplit : public Widget {
public:
    HSplit(int x, int y, int w, int h);
    HSplit();           //for full space
    ~HSplit() override;

    void Update() override;
    void Draw() override;

    void RecalculateAbsolutePosition() override;

    //Getters
    inline WidgetManager *Left() { return m_left; }
    inline WidgetManager *Right() { return m_right; }

private:
    void HandleMovingSeparator();
    void RecalculateSubMangerGeometries();
    int SeparatorAbsoluteX();

    WidgetManager *m_left;
    WidgetManager *m_right;
    float m_separator;      //value between 0 and 1

    bool m_occupy_all_space;
    int m_full_size_offset_top,
        m_full_size_offset_left,
        m_full_size_offset_right,
        m_full_size_offset_bottom;

    bool m_follow_mouse;
};


#endif //UI_FRAMEWORK_HSPLIT_H
