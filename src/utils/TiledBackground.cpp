#include <cmath>
#include "TiledBackground.h"

#include "engine/util/Trace.h"

TiledBackground::TiledBackground(Texture *texture) {
    m_texture = texture;
    m_scroll_speed = 16.f;
    m_color = WHITE;
    if(m_texture == nullptr) {
        TRACE("texture is null\n");
    }
}

void TiledBackground::Update(float dt) {
    m_scroll.x += m_scroll_speed*dt;
    m_scroll.y += m_scroll_speed*dt;

    if(m_texture == nullptr) return;

    m_scroll.x = fmod(m_scroll.x, (double)m_texture->width);
    m_scroll.y = fmod(m_scroll.y, (double)m_texture->height);
}

void TiledBackground::Draw() {
    if(m_texture == nullptr) return;
    if(m_texture->width == 0 || m_texture->height == 0) return;

#ifdef __EMSCRIPTEN__
    float w = (float)m_texture->width;
    float h = (float)m_texture->height;
    int x_stop = GetScreenWidth() / m_texture->width + 2;
    int y_stop = GetScreenHeight() / m_texture->height + 2;
    for(int y = 0; y < y_stop; ++y) {
        for(int x = 0; x < x_stop; ++x) {
            DrawTexturePro(*m_texture, {0.f, 0.f, w, h},
                           {-m_scroll.x + (float)x * w, -m_scroll.y + (float)y * h, w, h},
                           {0.f, 0.f}, 0.f, WHITE);
        }
    }
#else
    float w = (float)GetScreenWidth();
    float h = (float)GetScreenHeight();
    DrawTexturePro(*m_texture, {m_scroll.x, m_scroll.y, w, h}, //TODO : don't hardcode screen size ?
                   {0.f, 0.f, w, h},
                   {0.f, 0.f}, 0.f, m_color);
#endif
}

void TiledBackground::SetColor(Color c) {
    m_color = c;
}
