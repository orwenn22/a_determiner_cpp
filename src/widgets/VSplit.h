#ifndef UI_FRAMEWORK_VSPLIT_H
#define UI_FRAMEWORK_VSPLIT_H


#include "Widget.h"

class VSplit : public Widget {
public:
    VSplit(int x, int y, int w, int h);
    VSplit();           //for full space
    ~VSplit() override;

    void Update() override;
    void Draw() override;

    void RecalculateAbsolutePosition() override;

    //Getters
    inline WidgetManager *Top() { return m_top; }
    inline WidgetManager *Bottom() { return m_bottom; }

private:
    void HandleMovingSeparator();
    void RecalculateSubMangerGeometries();
    int SeparatorAbsoluteY();

    WidgetManager *m_top;
    WidgetManager *m_bottom;
    float m_separator;      //value between 0 and 1

    bool m_follow_mouse;
};


#endif //UI_FRAMEWORK_VSPLIT_H
