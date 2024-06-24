#include "TileGrid.h"

#include <malloc.h>

#include "metrics/Graphics.h"
#include "util/Math.h"
#include "Tileset.h"


TileGrid::TileGrid(int grid_width, int grid_height) {
    //TODO : size check ?
    m_grid_width = grid_width;
    m_grid_height = grid_height;
    m_data = (unsigned char *) malloc(sizeof(unsigned char) * m_grid_width * m_grid_height);

    size_t i_stop = m_grid_width*m_grid_height;
    for(size_t i = 0; i < i_stop; ++i) m_data[i] = 0;
}

unsigned char TileGrid::GetTile(int x, int y) {
    if(x < 0 || y < 0 || x >= m_grid_width || y >= m_grid_height) return 0;
    return UnsafeGetTile(x, y);
}

void TileGrid::SetTile(int x, int y, unsigned char value) {
    if(x < 0 || y < 0 || x >= m_grid_width || y >= m_grid_height) return;
    m_data[x + y*m_grid_width] = value;
}

void TileGrid::SetTile(size_t index, unsigned char value) {
    if(index >= m_grid_width*m_grid_height) return;
    m_data[index] = value;
}


void TileGrid::MDraw(Tileset *tileset, Rectangle dest) {
    if(tileset == nullptr || !tileset->Usable()) return;

    MetricsCamera *cam = Metrics::GetGraphicsCam();
    if(cam == nullptr) return;
    Vector2 cam_top_left = cam->ConvertAbsoluteToMeters(0, 0);
    Vector2 cam_bottom_right = cam->ConvertAbsoluteToMeters(GetScreenWidth(), GetScreenHeight());

    float tile_width = dest.width / (float)m_grid_width;
    float tile_height = dest.height / (float)m_grid_height;

    //Determine the interval of tiles we want to render
    Vector2i start = {
            (int)((cam_top_left.x - dest.x) / tile_width),
            (int)((cam_top_left.y - dest.y) / tile_height)
    };
    Vector2i stop = {
            (int)((cam_bottom_right.x - dest.x) / tile_width),
            (int)((cam_bottom_right.y - dest.y) / tile_height)
    };

    stop.x += 1; stop.y += 1;
    if(start.x < 0) start.x = 0;
    if(start.y < 0) start.y = 0;
    if(stop.x > m_grid_width) stop.x = m_grid_width;
    if(stop.y > m_grid_height) stop.y = m_grid_height;

    //Render the tiles
    for(int y = start.y; y < stop.y; ++y) {
        for(int x = start.x; x < stop.x; ++x) {
            //Calculate destination
            Rectangle tile_dest = {
                    dest.x + tile_width*(float)x,
                    dest.y + tile_height*(float)y,
                    tile_width,
                    tile_height
            };

            //Draw the tile
            tileset->MDraw(UnsafeGetTile(x, y), tile_dest);
        }
    }
}
