#include "HLine.h"

HLine::HLine(int x, int y, int w, Color c) : Widget(x, y, w, 1) {
    m_color = c;
}

HLine::~HLine() = default;

void HLine::Draw() {
    //Widget::Draw();
    int x = AbsoluteX();
    int y = AbsoluteY();

    DrawLine(x, y, x+Width(), y, m_color);
}
