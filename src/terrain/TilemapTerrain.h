#ifndef UI_FRAMEWORK_TILEMAPTERRAIN_H
#define UI_FRAMEWORK_TILEMAPTERRAIN_H

#include "engine/util/Math.h"
#include "Terrain.h"

#include <cstddef>


class Tileset;

class TilemapTerrain : public Terrain {
public:
    static TilemapTerrain *construct(const char *tileset_path, Vector2 size, int tile_width, int tile_height, int grid_width, int grid_height);
    ~TilemapTerrain() override;

    void SetTile(size_t tile_index, unsigned char value, unsigned char col_id);
    void SetTile(int x, int y, unsigned char value, unsigned char col_id);

    void Draw() override;
    void DrawGrid(Color c);

    bool CheckCollision(Vector2 position, bool outside_solid) override;
    bool CheckCollisionRec(Rectangle rec, bool outside_solid) override;

    void DestroyRectangle(Rectangle rec) override;
    void DestroyCircle(Vector2 center, float radius) override;       //Take in meters

    float Width() override;
    float Height() override;

    inline int GridWidth() { return m_grid_width; }
    inline int GridHeight() { return m_grid_height; }
    inline Tileset *GetTileset() { return m_tileset; }

    Vector2i GetTilePosition(Vector2 meter_position);

private:
    TilemapTerrain(const char *tileset_path, Vector2 size, int tile_width, int tile_height, int grid_width, int grid_height);

    void DestroyElispeTiles(Vector2 center, int radius_width, int radius_height);


    //TODO : method for setting only the size in meter, only for resizing, and only for changing the px size of a tile ?
    void SetTileset(Tileset *tileset);
    void SetGridSize(int w, int h, Vector2 size_m);


    Tileset *m_tileset;
    float m_tile_width_m, m_tile_height_m;      //Size of a tile in meter
    Vector2 m_size;                             //Size of the terrain in meter

    int m_grid_width, m_grid_height;            //Number of tile horizontally and vertically in the terrain
    unsigned char *m_tilemap_data;
    unsigned char *m_collision_mask;
};


#endif //UI_FRAMEWORK_TILEMAPTERRAIN_H
