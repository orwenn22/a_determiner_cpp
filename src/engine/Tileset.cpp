#include "Tileset.h"

#include "metrics/Graphics.h"

Tileset::Tileset(TextureRef texture, int tile_width, int tile_height) {
    m_tile_width = tile_width;
    if(m_tile_width <= 0) m_tile_width = 1;
    m_tile_height = tile_height;
    if(m_tile_height <= 0) m_tile_height = 1;

    m_texture = texture;

    if(!m_texture.IsValid()) {
        m_tile_count_x = 0;
        m_tile_count_y = 0;
    }
    else {
        m_tile_count_x = m_texture.GetTexturePtr()->width/m_tile_width;
        m_tile_count_y = m_texture.GetTexturePtr()->height/m_tile_height;
    }
}

Tileset::~Tileset() = default;

Tileset *Tileset::Clone() {
    Tileset *r = new Tileset(m_texture, m_tile_width, m_tile_height);
    return r;
}

void Tileset::SetTileSize(int tile_width, int tile_height) {
    m_tile_width = tile_width;
    if(m_tile_width <= 0) m_tile_width = 1;
    m_tile_height = tile_height;
    if(m_tile_height <= 0) m_tile_height = 1;

    m_tile_count_x = m_texture.GetTexturePtr()->width/m_tile_width;
    m_tile_count_y = m_texture.GetTexturePtr()->height/m_tile_height;
}


void Tileset::Draw(int index, Rectangle dest) {
    if(index >= m_tile_count_x*m_tile_count_y) return;
    float x = (float)(index%m_tile_count_x * m_tile_width);
    float y = (float)(index/m_tile_count_x * m_tile_height);

    //TODO : This is probably SUUUUUUUUPER slow, because the texture is copied to stack memory each time we draw a tile.
    //       Maybe it could be a good idea to make the m_texture of a TextureRef public to speed things up ?
    DrawTexturePro(m_texture.GetTexture(), {x, y, (float)m_tile_width, (float)m_tile_height},
                   dest,
                   {0.f, 0.f}, 0.f,
                   WHITE);
}

void Tileset::MDraw(int index, Rectangle dest) {
    //if(index >= m_tile_count_x*m_tile_count_y) return;
    float x = (float)(index%m_tile_count_x * m_tile_width);
    float y = (float)(index/m_tile_count_x * m_tile_height);

    //TODO : This is probably SUUUUUUUUPER slow, because the texture is copied to stack memory each time we draw a tile.
    //       Maybe it could be a good idea to make the m_texture of a TextureRef public to speed things up ?
    Metrics::DrawSpriteScaleEx(m_texture.GetTexture(), {x, y, (float)m_tile_width, (float)m_tile_height},
                   dest,
                   WHITE);
}

bool Tileset::Usable() {
    return (m_texture.IsValid() && m_tile_count_x != 0);
}

void Tileset::SetTexture(TextureRef texture) {
    m_texture = texture;

    if(!m_texture.IsValid()) {
        m_tile_count_x = 0;
        m_tile_count_y = 0;
    }
    else {
        m_tile_count_x = m_texture.GetTexturePtr()->width/m_tile_width;
        m_tile_count_y = m_texture.GetTexturePtr()->height/m_tile_height;
    }
}
