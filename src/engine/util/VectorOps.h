#ifndef UI_FRAMEWORK_VECTOROPS_H
#define UI_FRAMEWORK_VECTOROPS_H

#include <raylib.h>

// If there are problems with this, remove the inlines

// Addition
inline Vector2 operator+(Vector2& a, Vector2& b) { return {a.x+b.x, a.y+b.y}; }
//inline Vector2 operator+(Vector2& a, Vector2 b) { return {a.x+b.x, a.y+b.y}; }


// Multiplication
inline Vector2 operator*(Vector2& a, float b) {
    return {a.x*b, a.y*b};
}

// Division
inline Vector2 operator/(Vector2& a, float b) {
    return {a.x/b, a.y/b};
}

#endif //UI_FRAMEWORK_VECTOROPS_H
