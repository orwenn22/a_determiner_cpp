#ifndef UI_FRAMEWORK_TOOLTIPHORIZONTALALIGNMENT_H
#define UI_FRAMEWORK_TOOLTIPHORIZONTALALIGNMENT_H


#include "TooltipElement.h"

#include <vector>

class TooltipHorizontalAlignment : public TooltipElement {
public:
    TooltipHorizontalAlignment(int separation = 1);
    ~TooltipHorizontalAlignment() override;

    void Draw(int x, int y) override;

    void AddElement(TooltipElement *element);

private:
    std::vector<TooltipElement *> m_elements;
    int m_separation;
};


#endif //UI_FRAMEWORK_TOOLTIPHORIZONTALALIGNMENT_H
