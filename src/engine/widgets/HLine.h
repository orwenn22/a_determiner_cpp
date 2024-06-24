#ifndef UI_FRAMEWORK_HLINE_H
#define UI_FRAMEWORK_HLINE_H

#include "Widget.h"

#include <raylib.h>


class HLine : public Widget {
public:
    HLine(int x, int y, int w, Color c);
    ~HLine() override;

    void Draw() override;

private:
    Color m_color;
};


#endif //UI_FRAMEWORK_HLINE_H
