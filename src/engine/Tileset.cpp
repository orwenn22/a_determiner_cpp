#include "Tileset.h"

#include "metrics/Graphics.h"

Tileset::Tileset(Texture *texture, int tile_width, int tile_height, bool ownership) {
    m_ownership = ownership;
    m_tile_width = tile_width;
    if(m_tile_width <= 0) m_tile_width = 1;
    m_tile_height = tile_height;
    if(m_tile_height <= 0) m_tile_height = 1;

    if(texture == nullptr) {
        m_texture = nullptr;
        m_tile_count_x = 0;
        m_tile_count_y = 0;
    }
    else {
        m_texture = new Texture(*texture);
        m_tile_count_x = m_texture->width/m_tile_width;
        m_tile_count_y = m_texture->height/m_tile_height;
    }
}

Tileset::~Tileset() {
    if(m_ownership && m_texture != nullptr) UnloadTexture(*m_texture);
    delete m_texture;
}


void Tileset::SetTileSize(int tile_width, int tile_height) {
    m_tile_width = tile_width;
    if(m_tile_width <= 0) m_tile_width = 1;
    m_tile_height = tile_height;
    if(m_tile_height <= 0) m_tile_height = 1;

    m_tile_count_x = m_texture->width/m_tile_width;
    m_tile_count_y = m_texture->height/m_tile_height;
}


void Tileset::Draw(int index, Rectangle dest) {
    float x = (float)(index%m_tile_count_x * m_tile_width);
    float y = (float)(index/m_tile_count_x * m_tile_height);

    DrawTexturePro(*m_texture, {x, y, (float)m_tile_width, (float)m_tile_height},
                   dest,
                   {0.f, 0.f}, 0.f,
                   WHITE);
}

void Tileset::MDraw(int index, Rectangle dest) {
    float x = (float)(index%m_tile_count_x * m_tile_width);
    float y = (float)(index/m_tile_count_x * m_tile_height);

    Metrics::DrawSpriteScaleEx(*m_texture, {x, y, (float)m_tile_width, (float)m_tile_height},
                   dest,
                   WHITE);
}

bool Tileset::Usable() {
    return (m_texture != nullptr && m_tile_count_x != 0);
}

void Tileset::SetTexture(Texture *new_texture, bool new_ownership) {
    if(m_ownership && m_texture != nullptr) UnloadTexture(*m_texture);
    delete m_texture;

    m_ownership = new_ownership;
    if(new_texture == nullptr) {
        m_texture = nullptr;
        m_tile_count_x = 0;
        m_tile_count_y = 0;
    }
    else {
        m_texture = new Texture(*new_texture);
        m_tile_count_x = m_texture->width/m_tile_width;
        m_tile_count_y = m_texture->height/m_tile_height;
    }
}
