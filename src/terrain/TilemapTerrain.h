#ifndef A_DETERMINER_TILEMAPTERRAIN_H
#define A_DETERMINER_TILEMAPTERRAIN_H

#include "engine/util/Math.h"
#include "Terrain.h"

#include <cstddef>


class TileGrid;
class Tileset;

class TilemapTerrain : public Terrain {
public:
    static TilemapTerrain *construct(const char *tileset_path, Vector2 size, int tile_width, int tile_height, int grid_width, int grid_height);
    TilemapTerrain(Vector2 size, int grid_width, int grid_height);
    ~TilemapTerrain() override;

    void SetTile(size_t tile_index, unsigned char value, unsigned char col_id);
    void SetTile(int x, int y, unsigned char value, unsigned char col_id);
    void DestroyTile(int x, int y);

    void Draw() override;
    void DrawCollisions();

    bool CheckCollision(Vector2 position, bool outside_solid) override;
    bool CheckCollisionRec(Rectangle rec, bool outside_solid) override;

    void DestroyRectangle(Rectangle rec) override;
    void DestroyCircle(Vector2 center, float radius) override;       //Take in meters

    //Get terrain size in meter
    float Width() override;
    float Height() override;

    //Get the amount of tile horizontally/vertically
    int GridWidth();
    int GridHeight();

    inline Tileset *GetTileset() { return m_tileset; }

    Vector2i GetTilePosition(Vector2 meter_position);

    inline TileGrid *GetTilemap() { return m_tilemap_data; }
    inline TileGrid *GetCollision() { return m_collision_mask; }

    //Get the size of a tile in meters
    inline float TileWidth() { return m_tile_width_m; }
    inline float TileHeight() { return m_tile_height_m; }

    void SetTileset(Tileset *tileset);


private:
    TilemapTerrain(const char *tileset_path, Vector2 size, int tile_width, int tile_height, int grid_width, int grid_height);

    void DestroyElispeTiles(Vector2 center, int radius_width, int radius_height);


    //TODO : method for setting only the size in meter, only for resizing, and only for changing the px size of a tile ?
    void SetGridSize(int w, int h, Vector2 size_m);


    Tileset *m_tileset;
    float m_tile_width_m, m_tile_height_m;      //Size of a tile in meter
    Vector2 m_size;                             //Size of the terrain in meter

    TileGrid *m_tilemap_data;
    TileGrid *m_collision_mask;
};


#endif //A_DETERMINER_TILEMAPTERRAIN_H
