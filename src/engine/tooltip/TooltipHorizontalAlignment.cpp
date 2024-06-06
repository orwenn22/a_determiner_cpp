#include "TooltipHorizontalAlignment.h"

TooltipHorizontalAlignment::TooltipHorizontalAlignment(int separation) {
    m_width = 0;
    m_height = 0;
    m_separation = separation;
}

TooltipHorizontalAlignment::~TooltipHorizontalAlignment() {
    for(TooltipElement *e : m_elements) delete e;
    m_elements.clear();
}

void TooltipHorizontalAlignment::Draw(int x, int y) {
    int x_painter = x;
    for(TooltipElement *e : m_elements) {
        e->Draw(x_painter,  y + (m_height/2) - (e->Height()/2));
        x_painter += e->Width() + m_separation;
    }
}

void TooltipHorizontalAlignment::AddElement(TooltipElement *element) {
    if(element == nullptr) return;
    if(element->Height() > m_height) m_height = element->Height();
    m_width += element->Width() + m_separation*m_elements.empty();
    m_elements.push_back(element);
}
