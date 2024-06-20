#ifndef UI_FRAMEWORK_MATH_H
#define UI_FRAMEWORK_MATH_H

struct Vector2i {
    int x, y;

    Vector2i(int _x, int _y) : x(_x), y(_y) {}
};

template <typename T, typename U>
U sign(T value) {
    return (U) (T(0) < value) - (value < T(0));
}

#endif //UI_FRAMEWORK_MATH_H
