#ifndef UI_FRAMEWORK_TOOLTIPTEXT_H
#define UI_FRAMEWORK_TOOLTIPTEXT_H

#include "TooltipElement.h"

#include <raylib.h>
#include <string>

class TooltipText : public TooltipElement {
public:
    TooltipText(std::string text, int fontsize = 10, Color color = WHITE);
    ~TooltipText() override;

    void Draw(int x, int y) override;

private:
    std::string m_text;
    int m_fontsize;
    Color m_color;
};


#endif //UI_FRAMEWORK_TOOLTIPTEXT_H
