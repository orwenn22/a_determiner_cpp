#ifndef UI_FRAMEWORK_TERRAIN_H
#define UI_FRAMEWORK_TERRAIN_H

#include <raylib.h>

class Terrain {
public:
    Terrain();
    virtual ~Terrain();

    virtual void Draw() = 0;

    virtual bool CheckCollision(Vector2 position, bool outside_solid) = 0;
    virtual bool CheckCollisionRec(Rectangle rec, bool outside_solid) = 0;

    virtual void DestroyRectangle(Rectangle rec) = 0;
    virtual void DestroyCircle(Vector2 center, float radius) = 0;       //Take in meters

    virtual float Width() = 0;
    virtual float Height() = 0;

    inline float X() { return m_origin.x; }
    inline float Y() { return m_origin.y; }


protected:
    Vector2 m_origin;
};


#endif //UI_FRAMEWORK_TERRAIN_H
