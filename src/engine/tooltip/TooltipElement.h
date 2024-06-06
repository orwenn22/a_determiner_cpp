#ifndef UI_FRAMEWORK_TOOLTIPELEMENT_H
#define UI_FRAMEWORK_TOOLTIPELEMENT_H


class TooltipElement {
public:
    TooltipElement();
    virtual ~TooltipElement();

    virtual void Draw(int x, int y) = 0;

    int Width();
    int Height();

protected:
    int m_width;
    int m_height;
};


#endif //UI_FRAMEWORK_TOOLTIPELEMENT_H
