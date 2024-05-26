#ifndef UI_FRAMEWORK_TERRAIN_H
#define UI_FRAMEWORK_TERRAIN_H

#include <cstdint>
#include <raylib.h>


class Terrain {
public:
    static Terrain *construct(const char *filepath, Vector2 size);
    ~Terrain();

    void Draw();

    bool CheckCollision(Vector2 position, bool outside_solid = false);
    bool CheckCollisionRec(Rectangle rec, bool outside_solid = false);

    void DestroyRectangle(Rectangle rec);
    void DestroyCircle(Vector2 center, float radius);       //Take in meters

private:
    Terrain(const char *filepath, Vector2 size);

    //THIS DOES NOT PERFORM SAFETY CHECKS
    inline void UnsafeDestroyPixel(int x, int y);

    void DestroyElispePixel(Vector2 center, int radius_width, int radius_height);

    void UpdateSprite();


    Image m_image;              //on cpu
    Texture m_texture;          //on gpu
    uint8_t *m_collision_mask;  //collisions
    Vector2 m_size;             //in m
};


#endif //UI_FRAMEWORK_TERRAIN_H
