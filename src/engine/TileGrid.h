#ifndef UI_FRAMEWORK_TILEGRID_H
#define UI_FRAMEWORK_TILEGRID_H

#include <cstddef>
#include <raylib.h>

class Tileset;

class TileGrid {
public:
    TileGrid(int grid_width, int grid_height);
    ~TileGrid();

    unsigned char GetTile(int x, int y);
    inline unsigned char UnsafeGetTile(int x, int y) { return m_data[x + y*m_grid_width]; }

    void SetTile(int x, int y, unsigned char value);
    void SetTile(size_t index, unsigned char value);

    void MDraw(Tileset *tileset, Rectangle dest);

    inline int GridWidth() { return m_grid_width; }
    inline int GridHeight() { return m_grid_height; }

private:
    unsigned char *m_data;
    int m_grid_width;
    int m_grid_height;
};


#endif //UI_FRAMEWORK_TILEGRID_H
