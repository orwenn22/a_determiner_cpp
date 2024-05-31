#include <cmath>
#include "TiledBackground.h"

#include "engine/util/Trace.h"

TiledBackground::TiledBackground(Texture *texture) {
    m_texture = texture;
    m_scroll_speed = 16.f;
    if(m_texture == nullptr) {
        TRACE("texture is null\n");
    }
}

void TiledBackground::Update(float dt) {
    m_scroll.x += m_scroll_speed*dt;
    m_scroll.y += m_scroll_speed*dt;

    if(m_texture == nullptr) return;

    fmod(m_scroll.x, (double)m_texture->width);
    fmod(m_scroll.y, (double)m_texture->height);
}

void TiledBackground::Draw() {
    if(m_texture == nullptr) return;

    float w = (float)GetScreenWidth();
    float h = (float)GetScreenHeight();

    DrawTexturePro(*m_texture, {m_scroll.x, m_scroll.y, w, h}, //TODO : don't hardcode screen size ?
                   {0.f, 0.f, w, h},
                   {0.f, 0.f}, 0.f, WHITE);
}
