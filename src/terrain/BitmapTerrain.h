#ifndef UI_FRAMEWORK_BITMAPTERRAIN_H
#define UI_FRAMEWORK_BITMAPTERRAIN_H

#include "Terrain.h"

#include <cstdint>
#include <raylib.h>


class BitmapTerrain : public Terrain {
public:
    static BitmapTerrain *construct(const char *filepath, Vector2 size);
    ~BitmapTerrain() override;

    void Draw() override;

    bool CheckCollision(Vector2 position, bool outside_solid) override;
    bool CheckCollisionRec(Rectangle rec, bool outside_solid) override;

    void DestroyRectangle(Rectangle rec) override;
    void DestroyCircle(Vector2 center, float radius) override;       //Take in meters

    float Width() override { return m_size.x; }
    float Height() override { return m_size.y; }

    void MakeIndestructible(int x, int y, int w, int h);

private:
    BitmapTerrain(const char *filepath, Vector2 size);

    //THIS DOES NOT PERFORM SAFETY CHECKS
    inline void UnsafeDestroyPixel(int x, int y);

    void DestroyElispePixel(Vector2 center, int radius_width, int radius_height);

    void UpdateSprite();


    Vector2 m_origin;

    Image m_image;              //on cpu
    Texture m_texture;          //on gpu
    uint8_t *m_collision_mask;  //collisions
    Vector2 m_size;             //in m
};


#endif //UI_FRAMEWORK_BITMAPTERRAIN_H
