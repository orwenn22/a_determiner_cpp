#ifndef UI_FRAMEWORK_TILESET_H
#define UI_FRAMEWORK_TILESET_H

#include <raylib.h>

class Tileset {
public:
    Tileset(Texture *texture, int tile_width, int tile_height, bool ownership);
    ~Tileset();

    Tileset *WeakCopy();

    inline Texture* GetTexture() { return m_texture; }
    inline int GetTileWidth() { return m_tile_width; }
    inline int GetTileHeight() { return m_tile_height; }

    void SetTileSize(int tile_width, int tile_height);

    void Draw(int index, Rectangle dest);
    void MDraw(int index, Rectangle dest);

    bool Usable();

    void SetTexture(Texture *new_texture, bool new_ownership);

private:
    Texture *m_texture;
    int m_tile_width, m_tile_height;
    int m_tile_count_x, m_tile_count_y;

    bool m_ownership;
};


#endif //UI_FRAMEWORK_TILESET_H
