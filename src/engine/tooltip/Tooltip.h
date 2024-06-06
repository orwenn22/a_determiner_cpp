#ifndef UI_FRAMEWORK_TOOLTIP_H
#define UI_FRAMEWORK_TOOLTIP_H

#include <vector>

class TooltipElement;

class Tooltip {
public:
    Tooltip();
    virtual ~Tooltip();

    void Draw(int x, int y);

    void AddElement(TooltipElement *element);
    void ClearElements();

private:
    std::vector<TooltipElement *> m_elements;
};


#endif //UI_FRAMEWORK_TOOLTIP_H
