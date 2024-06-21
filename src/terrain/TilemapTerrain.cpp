#include "TilemapTerrain.h"

#include <cmath>
#include <cstdio>
#include <malloc.h>

#include "engine/metrics/Graphics.h"
#include "engine/metrics/MetricsCamera.h"


TilemapTerrain *TilemapTerrain::construct(const char *tileset_path, Vector2 size, int tile_width, int tile_height, int grid_width, int grid_height) {
    if(tile_width <= 0 || tile_height <= 0 || size.x <= 0 || size.y <= 0 || grid_width <= 0 ||grid_height <= 0) return nullptr;

    FILE *in_file = fopen(tileset_path, "r");
    if(in_file == nullptr) return nullptr;
    fclose(in_file);

    return new TilemapTerrain(tileset_path, size, tile_width, tile_height, grid_width, grid_height);
}

TilemapTerrain::TilemapTerrain(const char *tileset_path, Vector2 size, int tile_width, int tile_height, int grid_width, int grid_height) {
    m_origin = {0.f, 0.f};
    m_tileset = nullptr;
    SetTileset(new Texture(LoadTexture(tileset_path)), tile_width, tile_height);

    //This if fine, because these get set in SetGridSize
    m_grid_width = 0;
    m_grid_height = 0;
    m_tilemap_data = nullptr;
    m_collision_mask = nullptr;

    SetGridSize(grid_width, grid_height, size);
}

TilemapTerrain::~TilemapTerrain() {
    if(m_tileset != nullptr) {
        UnloadTexture(*m_tileset);
        delete m_tileset;
    }
    free(m_tilemap_data);
    free(m_collision_mask);
}


void TilemapTerrain::SetTile(size_t tile_index, unsigned char value, unsigned char col_id) {
    if(tile_index >= m_grid_width*m_grid_height) return;
    m_tilemap_data[tile_index] = value;
    m_collision_mask[tile_index] = col_id;
}

void TilemapTerrain::SetTile(int x, int y, unsigned char value, unsigned char col_id) {
    if(x < 0 || y < 0 || x >= m_grid_width || y >= m_grid_height) return;
    size_t index = x+y*m_grid_width;
    SetTile(index, value, col_id);
}

void TilemapTerrain::Draw() {
    if(m_tileset == nullptr) {
        //Red outline
        Metrics::DrawRectangle(m_origin.x, m_origin.y, m_size.x, m_size.y, RED, false);
        return;
    }

    MetricsCamera *cam = Metrics::GetGraphicsCam();
    if(cam == nullptr) return;

    //Determine the interval of tiles we want to render
    Vector2i start = GetTilePosition(cam->ConvertAbsoluteToMeters(0, 0));
    Vector2i stop = GetTilePosition(cam->ConvertAbsoluteToMeters(GetScreenWidth(), GetScreenHeight()));
    stop.x += 1; stop.y += 1;
    if(start.x < 0) start.x = 0;
    if(start.y < 0) start.y = 0;
    if(stop.x > m_grid_width) stop.x = m_grid_width;
    if(stop.y > m_grid_height) stop.y = m_grid_height;

    //Render the tiles
    for(int y = start.y; y < stop.y; ++y) {
        for(int x = start.x; x < stop.x; ++x) {
            //Get the tile index
            unsigned char tile_index = m_tilemap_data[x+y*m_grid_width];

            //Calculate source and destination
            Rectangle source = {
                    (float)(m_tile_width * (tile_index%m_tile_count_x)),
                    (float)(m_tile_height * (tile_index/m_tile_count_x)),
                    (float)m_tile_width,
                    (float)m_tile_height
            };
            Rectangle dest = {
                    m_origin.x + m_tile_width_m*(float)x,
                    m_origin.y + m_tile_height_m*(float)y,
                    m_tile_width_m,
                    m_tile_height_m
            };

            //Draw the tile
            Metrics::DrawSpriteScaleEx(*m_tileset, source, dest, WHITE);
        }
    }

    //Red outline
    Metrics::DrawRectangle(m_origin.x, m_origin.y, m_size.x, m_size.y, RED, false);
}

void TilemapTerrain::DrawGrid(Color c) {
    //TODO : implement this
}


bool TilemapTerrain::CheckCollision(Vector2 position, bool outside_solid) {
    //Compute the coordinates of the correct tile on the tilemap
    int tile_x = (int) ((position.x - m_origin.x)/m_size.x * (float)m_grid_width) - (position.x < m_origin.x);
    int tile_y = (int) ((position.y - m_origin.y)/m_size.y * (float)m_grid_height) - (position.y < m_origin.y);

    if(tile_x < 0 || tile_x >= m_grid_width || tile_y < 0 || tile_y >= m_grid_height) return outside_solid;
    return m_collision_mask[tile_x + tile_y*m_grid_width];
}

bool TilemapTerrain::CheckCollisionRec(Rectangle rec, bool outside_solid) {
    //Top left
    int tile_x = (int) (rec.x / m_size.x * (float)m_grid_width) - (rec.x < m_origin.x);
    int tile_y = (int) (rec.y / m_size.y * (float)m_grid_height) - (rec.y < m_origin.y);

    //Bottom right
    int tile_x2 = (int) ((rec.x + rec.width - m_origin.x) / m_size.x * (float)m_grid_width) - (rec.x + rec.width < m_origin.x);
    int tile_y2 = (int) ((rec.y + rec.height - m_origin.y) / m_size.y * (float)m_grid_height) - (rec.y + rec.height < m_origin.y);

    if(outside_solid) {
        if(tile_x < 0 || tile_x2 >= m_grid_width || tile_y < 0 || tile_y2 >= m_grid_height) return true;
    }
    else {
        if(tile_x < 0) tile_x = 0;
        if(tile_x2 >= m_grid_width) tile_x2 = m_grid_width - 1;
        else if(tile_x2 < 0) tile_x2 = 0;     //Necessary because the "for" loops use size_t (unsigned)

        if(tile_y < 0) tile_y = 0;
        if(tile_y2 >= m_grid_height) tile_y2 = m_grid_height - 1;
        else if(tile_y2 < 0) tile_y2 = 0;     //Necessary because the "for" loops use size_t (unsigned)
    }

    //Check left and right edges
    if(!(tile_x >= m_grid_width || tile_x2 < 0)) {
        for(size_t y = tile_y; y <= tile_y2; ++y) {
            if(m_collision_mask[tile_x + y * m_grid_width] || m_collision_mask[tile_x2 + y * m_grid_width]) return true;
        }
    }

    //Check top and bottom edges
    if(!(tile_y >= m_grid_height || tile_y2 < 0)) {
        for(size_t x = tile_x; x <= tile_x2; ++x) {
            if(m_collision_mask[x + tile_y * m_grid_width] || m_collision_mask[x + tile_y2 * m_grid_width]) return true;
        }
    }

    //Check center
    for(size_t y = tile_y + 1; y < tile_y2; ++y) {
        for(size_t x = tile_x + 1; x < tile_x2; ++x) {
            if(m_collision_mask[x + y * m_grid_width]) return true;
        }
    }

    return false;
}


void TilemapTerrain::DestroyRectangle(Rectangle rec) {
    //Top left
    int tile_x = (int) (rec.x / m_size.x * (float)m_grid_width) - (rec.x < m_origin.x);
    int tile_y = (int) (rec.y / m_size.y * (float)m_grid_height) - (rec.y < m_origin.y);

    //Bottom right
    int tile_x2 = (int) ((rec.x + rec.width - m_origin.x) / m_size.x * (float)m_grid_width) - (rec.x + rec.width < m_origin.x);
    int tile_y2 = (int) ((rec.y + rec.height - m_origin.y) / m_size.y * (float)m_grid_height) - (rec.y + rec.height < m_origin.y);

    //Make sure we don't try to destroy outside
    if(tile_x < 0) tile_x = 0;
    if(tile_x2 >= m_grid_width) tile_x2 = m_grid_width - 1;
    if(tile_y < 0) tile_y = 0;
    if(tile_y2 >= m_grid_height) tile_y2 = m_grid_height - 1;

    for(size_t y = tile_y; y <= tile_y2; ++y) {
        for(size_t x = tile_x; x <= tile_x2; ++x) {
            SetTile((int)x, (int)y, 0, 0);
        }
    }
}

void TilemapTerrain::DestroyCircle(Vector2 center, float radius) {
    int radius_width = (int)(radius / m_size.x * (float)m_grid_width);
    int radius_height = (int)(radius / m_size.y * (float)m_grid_height);
    DestroyElispeTiles(center, radius_width, radius_height);
}


float TilemapTerrain::Width() { return m_size.x; }
float TilemapTerrain::Height() { return m_size.y; }


Vector2i TilemapTerrain::GetTilePosition(Vector2 meter_position) {
    int x = (int)((meter_position.x-m_origin.x)/m_tile_width_m) - (meter_position.x < m_origin.x);
    int y = (int)((meter_position.y-m_origin.y)/m_tile_height_m) - (meter_position.y < m_origin.y);
    return {x, y};
}


///////////////////////////////////////////////
//// PRIVATE

void TilemapTerrain::DestroyElispeTiles(Vector2 center, int radius_width, int radius_height) {
    int pixel_x = (int) ((center.x - m_origin.x)/m_size.x * (float)m_grid_width) - (center.x < m_origin.x);
    int pixel_y = (int) ((center.y - m_origin.y)/m_size.y * (float)m_grid_height) - (center.y < m_origin.y);

    //Number of iterations we need to do in order to not skip columns of pixels of the circle
    int step_count = (int)((float)radius_width * 3.14f);

    int previous_xpos = -99999;
    for(int i = 0; i < step_count; ++i) {
        //compute x position for the current step
        int x_pos = pixel_x + (int)(cos((double)i*PI / (double)step_count) * (double)radius_width);
        if(x_pos == previous_xpos) continue;        //don't do the same column multiple times
        previous_xpos = x_pos;

        if(x_pos < 0 || x_pos >= m_grid_width) continue;       //don't try to remove outside the terrain

        int y_dist = (int)(sin((double)i*PI / (double)step_count) * (double)radius_height);         //y distance from the x axes
        for(int y = pixel_y-y_dist; y <= pixel_y+y_dist; ++y) {                 //for each tile of the columns...
            if(y < 0 || y >= m_grid_height) continue;                           //check if it is in bound....
            SetTile(x_pos, y, 0, 0);                             //If so destroy it
        }
    }
}


void TilemapTerrain::SetTileset(Texture *tileset, int tile_width_px, int tile_height_px) {
    if(m_tileset != nullptr) {
        UnloadTexture(*m_tileset);
        delete m_tileset;
    }
    m_tileset = tileset;
    m_tile_width = tile_width_px;
    m_tile_height = tile_height_px;
    m_tile_count_x = m_tileset->width / m_tile_width;
    m_tile_count_y = m_tileset->height / m_tile_height;
}


/**
 * Configure the size of the terrain
 * @param w number of tile horizontally in the terrain
 * @param h number of tile vertically in the terrain
 * @param size_m size in meter of the terrain in the game world
 */
void TilemapTerrain::SetGridSize(int w, int h, Vector2 size_m) {
    m_size = size_m;
    if(w <= 0 || h <= 0) return;
    if(w == m_grid_width || h == m_grid_height) return;

    m_grid_width = w;
    m_grid_height = h;
    m_tile_width_m = m_size.x / (float)m_grid_width;
    m_tile_height_m = m_size.y / (float) m_grid_height;

    size_t count = m_grid_width*m_grid_height;
    unsigned char *new_tilemap_data = (unsigned char *) malloc(sizeof(unsigned char) * count);
    unsigned char *new_collision_mask = (unsigned char *) malloc(sizeof(unsigned char) * count);

    //if(m_tilemap_data == nullptr || m_collision_mask == nullptr) {
        for (size_t i = 0; i < count; ++i) {
            new_tilemap_data[i] = 0;
            new_collision_mask[i] = 0;
        }
    //}
    //else {
        //TODO : copy previous content here ?

        free(m_tilemap_data);
        free(m_collision_mask);
    //}

    m_tilemap_data = new_tilemap_data;
    m_collision_mask = new_collision_mask;
}
