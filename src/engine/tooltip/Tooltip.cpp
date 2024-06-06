#include "Tooltip.h"

#include "TooltipElement.h"

#include <raylib.h>

Tooltip::Tooltip() { }

Tooltip::~Tooltip() {
    ClearElements();
}

void Tooltip::Draw(int x, int y) {
    int width = 0;
    int height = 0;
    for(TooltipElement *e : m_elements) {
        height += e->Height() + 2;
        int w = e->Width();
        if(w > width) width = w;
    }

    if(width == 0 || height == 0) return;
    width += 4;
    height += 2;

    //TODO : make this independant of screen size
    if(x+width >= GetScreenWidth()) x-= (width+3);
    if(y+height >= GetScreenHeight()) y-= (height+3);

    DrawRectangle(x, y, width, height, BLACK);
    DrawRectangleLines(x, y, width, height, WHITE);

    int painter_y = 2;
    for(TooltipElement *e : m_elements) {
        e->Draw(x+2, y+painter_y);
        painter_y += e->Height() + 2;
    }
}

void Tooltip::AddElement(TooltipElement *element) {
    if(element == nullptr) return;
    m_elements.push_back(element);
}

void Tooltip::ClearElements() {
    for(TooltipElement *e : m_elements) delete e;
    m_elements.clear();
}
