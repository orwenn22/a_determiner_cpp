#include "TooltipText.h"

TooltipText::TooltipText(std::string text, int fontsize, Color color) {
    m_text = text;
    m_fontsize = fontsize;
    m_color = color;
    //m_width = MeasureText(text.c_str(), fontsize);
    //m_height = fontsize + ((fontsize+5) * (int)std::count(m_text.begin(), m_text.end(), '\n'));
    int spacing = fontsize/10;
    Vector2 text_size = MeasureTextEx(GetFontDefault(), text.c_str(), (float) fontsize, (float) spacing);
    m_width = (int) text_size.x;
    m_height = (int) text_size.y;
}

TooltipText::~TooltipText() { }

void TooltipText::Draw(int x, int y) {
    int spacing = m_fontsize/10;
    DrawTextEx(GetFontDefault(), m_text.c_str(), {(float)x, (float)y}, (float)m_fontsize, (float)spacing, m_color);
    DrawText(m_text.c_str(), x, y, m_fontsize, m_color);
}
