#include "TilemapTerrain.h"

#include <cmath>
#include <cstdio>
#include <malloc.h>

#include "engine/metrics/Graphics.h"


TilemapTerrain *TilemapTerrain::construct(const char *tileset_path, Vector2 size, int tile_width, int tile_height, int grid_width, int grid_height) {
    if(tile_width <= 0 || tile_height <= 0 || size.x <= 0 || size.y <= 0 || grid_width <= 0 ||grid_height <= 0) return nullptr;

    FILE *in_file = fopen(tileset_path, "r");
    if(in_file == nullptr) return nullptr;
    fclose(in_file);

    return new TilemapTerrain(tileset_path, size, tile_width, tile_height, grid_width, grid_height);
}

TilemapTerrain::TilemapTerrain(const char *tileset_path, Vector2 size, int tile_width, int tile_height, int grid_width, int grid_height) {
    m_origin = {0.f, 0.f};
    m_tileset = LoadTexture(tileset_path);
    m_size = size;
    m_tile_width = tile_width;
    m_tile_height = tile_height;
    m_tile_count_x = m_tileset.width / m_tile_width;
    m_tile_count_y = m_tileset.height / m_tile_height;
    printf("Tile count x : %i   tile count y : %i\n", m_tile_count_x, m_tile_count_y);
    m_grid_width = grid_width;
    m_grid_height = grid_height;

    size_t count = m_grid_width*m_grid_height;
    m_tilemap_data = (unsigned char *) malloc(sizeof(unsigned char) * count);
    m_collision_mask = (unsigned char *) malloc(sizeof(unsigned char) * count);
    for(size_t i = 0; i < count; ++i) {
        m_tilemap_data[i] = 0;
        m_collision_mask[i] = 0;
    }
}

TilemapTerrain::~TilemapTerrain() {
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
    //Calculate the size of a tile in meter
    float tile_w_m = m_size.x / (float)m_grid_width;
    float tile_h_m = m_size.y / (float)m_grid_height;

    for(size_t y = 0; y < m_grid_height; ++y) {
        for(size_t x = 0; x < m_grid_width; ++x) {
            //Get the tile index
            unsigned char tile_index = m_tilemap_data[x+y*m_grid_width];

            //Calculate source and destination
            Rectangle source = {
                    (float)(m_tile_width * (tile_index%m_tile_count_x)),
                    (float)(m_tile_height * (tile_index/m_tile_count_x)),
                    (float)m_tile_width,
                    (float)m_tile_height
            };
            Rectangle dest = {m_origin.x + tile_w_m*(float)x, m_origin.y + tile_h_m*(float)y, tile_w_m, tile_h_m};

            //Draw the tile
            Metrics::DrawSpriteScaleEx(m_tileset, source, dest, WHITE);
        }
    }

    //Red outline
    Metrics::DrawRectangle(m_origin.x, m_origin.y, m_size.x, m_size.y, RED, false);
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
