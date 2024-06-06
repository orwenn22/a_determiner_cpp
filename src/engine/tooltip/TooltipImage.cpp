#include "TooltipImage.h"

#include <raylib.h>

TooltipImage::TooltipImage(Texture *texture) {
    m_texture = texture;
    if(texture != nullptr) {
        m_width = m_texture->width;
        m_height = m_texture->height;
    }
    else {
        m_width = 0;
        m_height = 0;
    }
    //TODO : scaling options ?
}

TooltipImage::~TooltipImage() = default;

void TooltipImage::Draw(int x, int y) {
    DrawTexture(*m_texture, x, y, WHITE);
}
