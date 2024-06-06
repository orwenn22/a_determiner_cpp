#ifndef UI_FRAMEWORK_TOOLTIPIMAGE_H
#define UI_FRAMEWORK_TOOLTIPIMAGE_H


#include "TooltipElement.h"

#include <raylib.h>     //TODO : raylib dependant

class TooltipImage : public TooltipElement {
public:
    TooltipImage(Texture *texture);
    ~TooltipImage() override;

    void Draw(int x, int y) override;

private:
    Texture *m_texture;
};



#endif //UI_FRAMEWORK_TOOLTIPIMAGE_H
