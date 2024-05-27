#ifndef UI_FRAMEWORK_MATH_H
#define UI_FRAMEWORK_MATH_H

template <typename T, typename U>
U sign(T value) {
    return (U) (T(0) < value) - (value < T(0));
}

#endif //UI_FRAMEWORK_MATH_H
