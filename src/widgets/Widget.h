#ifndef UI_FRAMEWORK_WIDGET_H
#define UI_FRAMEWORK_WIDGET_H

class WidgetManager;

enum WidgetAlignment {
    WidgetAlignment_None    = 0x00,

    WidgetAlignment_HCenter = 0x01,
    WidgetAlignment_Right   = 0x02,

    WidgetAlignment_VCenter = 0x10,
    WidgetAlignment_Bottom  = 0x20,

    WidgetAlignment_RightCenter = WidgetAlignment_Right | WidgetAlignment_VCenter,
    WidgetAlignment_MiddleBottom = WidgetAlignment_HCenter | WidgetAlignment_Bottom,

    WidgetAlignment_Center  = WidgetAlignment_HCenter | WidgetAlignment_VCenter,
    WidgetAlignment_BottomRight = WidgetAlignment_Bottom | WidgetAlignment_Right,
};

class Widget {
public:
    Widget(int x, int y, int w, int h);
    virtual ~Widget();

    virtual void Update();
    virtual void Draw();

    virtual void RecalculateAbsolutePosition();

    bool IsMouseHovering();

    // Setters
    void SetX(int x);
    void SetY(int y);
    void SetPosition(int x, int y);
    void SetWidth(int w);
    void SetHeight(int h);
    void SetSize(int w, int h);
    void SetAlignment(WidgetAlignment alignment);

    //Getters
    inline int X() { return m_relative_x; }
    inline int Y() { return m_relative_y; }
    inline int AbsoluteX() { return m_absolute_x; }
    inline int AbsoluteY() { return m_absolute_y; }
    inline int Width() { return m_w; }
    inline int Height() { return m_h; }
    inline WidgetManager *GetManager() { return m_manager; }


protected:
    //Only access these directly if a subclass have a custom RecalculateAbsolutePosition
    int m_relative_x, m_relative_y;
    int m_absolute_x, m_absolute_y;
    int m_w, m_h;

private:
    friend WidgetManager;

    void SetManager(WidgetManager *manager);
    WidgetAlignment m_alignment;
    WidgetManager *m_manager;
};


#endif //UI_FRAMEWORK_WIDGET_H
