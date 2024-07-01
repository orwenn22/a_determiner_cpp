#include "TilemapTerrain.h"

#include <cmath>
#include <cstdio>
#include <malloc.h>

#include "engine/metrics/Graphics.h"
#include "engine/metrics/MetricsCamera.h"
#include "engine/TileGrid.h"
#include "engine/Tileset.h"
#include "GlobalResources.h"


TilemapTerrain *TilemapTerrain::construct(const char *tileset_path, Vector2 size, int tile_width, int tile_height, int grid_width, int grid_height) {
    if(tile_width <= 0 || tile_height <= 0 || size.x <= 0 || size.y <= 0 || grid_width <= 0 ||grid_height <= 0) return nullptr;

    FILE *in_file = fopen(tileset_path, "r");
    if(in_file == nullptr) return nullptr;
    fclose(in_file);

    return new TilemapTerrain(tileset_path, size, tile_width, tile_height, grid_width, grid_height);
}

TilemapTerrain::TilemapTerrain(const char *tileset_path, Vector2 size, int tile_width, int tile_height, int grid_width, int grid_height) {
    m_origin = {0.f, 0.f};
    Texture tileset_texture = LoadTexture(tileset_path);
    m_tileset = new Tileset(&tileset_texture, tile_width, tile_height, true);

    m_tilemap_data = nullptr;
    m_collision_mask = nullptr;
    SetGridSize(grid_width, grid_height, size);
}

TilemapTerrain::TilemapTerrain(Vector2 size, int grid_width, int grid_height) {
    m_origin = {0.f, 0.f};
    m_tileset = nullptr;
    m_tilemap_data = nullptr;
    m_collision_mask = nullptr;
    SetGridSize(grid_width, grid_height, size);
}

TilemapTerrain::~TilemapTerrain() {
    delete m_tileset;
    delete m_tilemap_data;
    delete m_collision_mask;
}


void TilemapTerrain::SetTile(size_t tile_index, unsigned char value, unsigned char col_id) {
    m_tilemap_data->SetTile(tile_index, value);
    m_collision_mask->SetTile(tile_index, col_id);
}

void TilemapTerrain::SetTile(int x, int y, unsigned char value, unsigned char col_id) {
    m_tilemap_data->SetTile(x, y, value);
    m_collision_mask->SetTile(x, y, col_id);
}

void TilemapTerrain::DestroyTile(int x, int y) {
    if(m_collision_mask->GetTile(x, y) == 2) return;        //2 is indestructible
    SetTile(x, y, 0, 0);
}


void TilemapTerrain::Draw() {
    if(m_tileset == nullptr) {
        //Red outline
        Metrics::DrawRectangle(m_origin.x, m_origin.y, m_size.x, m_size.y, RED, false);
        return;
    }

    MetricsCamera *cam = Metrics::GetGraphicsCam();
    if(cam == nullptr || m_tilemap_data == nullptr) return;
    m_tilemap_data->MDraw(m_tileset, {m_origin.x, m_origin.y, m_size.x, m_size.y});
    //DrawCollisions();

    //Red outline
    Metrics::DrawRectangle(m_origin.x, m_origin.y, m_size.x, m_size.y, RED, false);
}

void TilemapTerrain::DrawCollisions() {
    MetricsCamera *cam = Metrics::GetGraphicsCam();
    if(cam == nullptr || m_tilemap_data == nullptr) return;
    m_collision_mask->MDraw(Res::collisions_tileset, {m_origin.x, m_origin.y, m_size.x, m_size.y});
}


bool TilemapTerrain::CheckCollision(Vector2 position, bool outside_solid) {
    //Compute the coordinates of the correct tile on the tilemap
    Vector2i tile_pos = GetTilePosition(position);

    if(tile_pos.x < 0 || tile_pos.x >= m_collision_mask->GridWidth() || tile_pos.y < 0 || tile_pos.y >= m_collision_mask->GridHeight()) return outside_solid;
    return m_collision_mask->GetTile(tile_pos.x, tile_pos.y);
}

bool TilemapTerrain::CheckCollisionRec(Rectangle rec, bool outside_solid) {
    int grid_width = m_collision_mask->GridWidth();
    int grid_height = m_collision_mask->GridHeight();

    //Top left
    Vector2i tile_pos = GetTilePosition({rec.x, rec.y});

    //Bottom right
    Vector2i tile_pos2 = GetTilePosition({rec.x + rec.width, rec.y + rec.height});

    if(outside_solid) {
        if(tile_pos.x < 0 || tile_pos2.x >= grid_width || tile_pos.y < 0 || tile_pos2.y >= grid_height) return true;
    }
    else {
        if(tile_pos.x < 0) tile_pos.x = 0;
        if(tile_pos2.x >= grid_width) tile_pos2.x = grid_width - 1;
        else if(tile_pos2.x < 0) tile_pos2.x = 0;     //Necessary because the "for" loops use size_t (unsigned)

        if(tile_pos.y < 0) tile_pos.y = 0;
        if(tile_pos2.y >= grid_height) tile_pos2.y = grid_height - 1;
        else if(tile_pos2.y < 0) tile_pos2.y = 0;     //Necessary because the "for" loops use size_t (unsigned)
    }

    //Check left and right edges
    if(!(tile_pos.x >= grid_width || tile_pos2.x < 0)) {
        for(size_t y = tile_pos.y; y <= tile_pos2.y; ++y) {
            if(m_collision_mask->UnsafeGetTile(tile_pos.x, (int)y) || m_collision_mask->UnsafeGetTile(tile_pos2.x, (int)y)) return true;
        }
    }

    //Check top and bottom edges
    if(!(tile_pos.y >= grid_height || tile_pos2.y < 0)) {
        for(size_t x = tile_pos.x; x <= tile_pos2.x; ++x) {
            if(m_collision_mask->UnsafeGetTile((int)x, tile_pos.y) || m_collision_mask->UnsafeGetTile((int)x, tile_pos2.y)) return true;
        }
    }

    //Check center
    for(size_t y = tile_pos.y + 1; y < tile_pos2.y; ++y) {
        for(size_t x = tile_pos.x + 1; x < tile_pos2.x; ++x) {
            if(m_collision_mask->UnsafeGetTile((int)x, (int)y)) return true;
        }
    }

    return false;
}


void TilemapTerrain::DestroyRectangle(Rectangle rec) {
    //Top left
    Vector2i tile_pos = GetTilePosition({rec.x, rec.y});

    //Bottom right
    Vector2i tile_pos2 = GetTilePosition({rec.x + rec.width, rec.y + rec.height});

    //Make sure we don't try to destroy outside
    if(tile_pos.x < 0) tile_pos.x = 0;
    if(tile_pos2.x >= m_tilemap_data->GridWidth()) tile_pos2.x = m_tilemap_data->GridWidth() - 1;
    if(tile_pos.y < 0) tile_pos.y = 0;
    if(tile_pos2.y >= m_tilemap_data->GridHeight()) tile_pos2.y = m_tilemap_data->GridHeight() - 1;

    for(size_t y = tile_pos.y; y <= tile_pos2.y; ++y) {
        for(size_t x = tile_pos.x; x <= tile_pos2.x; ++x) {
            DestroyTile((int)x, (int)y);
        }
    }
}

void TilemapTerrain::DestroyCircle(Vector2 center, float radius) {
    int radius_width = (int)(radius / m_size.x * (float)m_tilemap_data->GridWidth());
    int radius_height = (int)(radius / m_size.y * (float)m_tilemap_data->GridHeight());
    DestroyElispeTiles(center, radius_width, radius_height);
}


float TilemapTerrain::Width() { return m_size.x; }
float TilemapTerrain::Height() { return m_size.y; }

int TilemapTerrain::GridWidth() { return (m_tilemap_data == nullptr) ? 0 : m_tilemap_data->GridWidth(); }
int TilemapTerrain::GridHeight() { return (m_tilemap_data == nullptr) ? 0 : m_tilemap_data->GridHeight(); }


Vector2i TilemapTerrain::GetTilePosition(Vector2 meter_position) {
    int x = (int)((meter_position.x-m_origin.x)/m_tile_width_m) - (meter_position.x < m_origin.x);
    int y = (int)((meter_position.y-m_origin.y)/m_tile_height_m) - (meter_position.y < m_origin.y);
    return {x, y};
}


void TilemapTerrain::SetTileset(Tileset *tileset) {
    delete m_tileset;
    m_tileset = tileset;
}


///////////////////////////////////////////////
//// PRIVATE

void TilemapTerrain::DestroyElispeTiles(Vector2 center, int radius_width, int radius_height) {
    int m_grid_width = m_tilemap_data->GridWidth();
    int m_grid_height = m_tilemap_data->GridHeight();
    //int tile_x = (int) ((center.x - m_origin.x) / m_size.x * (float)m_grid_width) - (center.x < m_origin.x);
    //int tile_y = (int) ((center.y - m_origin.y) / m_size.y * (float)m_grid_height) - (center.y < m_origin.y);
    Vector2i tile_pos = GetTilePosition(center);

    //Number of iterations we need to do in order to not skip columns of pixels of the circle
    int step_count = (int)((float)radius_width * 3.14f);

    int previous_xpos = -99999;
    for(int i = 0; i < step_count; ++i) {
        //compute x position for the current step
        int x_pos = tile_pos.x + (int)(cos((double)i * PI / (double)step_count) * (double)radius_width);
        if(x_pos == previous_xpos) continue;        //don't do the same column multiple times
        previous_xpos = x_pos;

        if(x_pos < 0 || x_pos >= m_grid_width) continue;       //don't try to remove outside the terrain

        int y_dist = (int)(sin((double)i*PI / (double)step_count) * (double)radius_height);         //y distance from the x axes
        for(int y = tile_pos.y - y_dist; y <= tile_pos.y + y_dist; ++y) {                 //for each tile of the columns...
            if(y < 0 || y >= m_grid_height) continue;                           //check if it is in bound....
            DestroyTile(x_pos, y);                                           //If so destroy it
        }
    }
}


/**
 * Configure the size of the terrain
 * @param w number of tile horizontally in the terrain
 * @param h number of tile vertically in the terrain
 * @param size_m size in meter of the terrain in the game world
 */
void TilemapTerrain::SetGridSize(int w, int h, Vector2 size_m) {
    //Recalculate tile sizes
    m_size = size_m;
    m_tile_width_m = m_size.x / (float)w;
    m_tile_height_m = m_size.y / (float)h;

    //Allocate with new size
    TileGrid *new_tilemap_data = new TileGrid(w, h);
    TileGrid *new_collision_mask = new TileGrid(w, h);

    //Copy previous data if possible
    if(m_tilemap_data != nullptr && m_collision_mask != nullptr) {
        for(int y = 0; y < m_tilemap_data->GridHeight(); ++y) {
            for(int x = 0; x < m_tilemap_data->GridWidth(); ++x) {
                new_tilemap_data->SetTile(x, y, m_tilemap_data->GetTile(x, y));
                new_collision_mask->SetTile(x, y, m_collision_mask->GetTile(x, y));
            }
        }
    }

    //Deallocate previous data and replace them with the new TileGrids
    delete m_tilemap_data;
    delete m_collision_mask;
    m_tilemap_data = new_tilemap_data;
    m_collision_mask = new_collision_mask;

}
