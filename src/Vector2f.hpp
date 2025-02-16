#ifndef VECTOR2F_HPP
#define VECTOR2F_HPP

#include <SDL2/SDL.h>

struct Vector2f {
    float x, y;

    Vector2f() : x(0), y(0) {}
    Vector2f(float x, float y) : x(x), y(y) {}

    Vector2f operator+(const Vector2f& other) const {
        return Vector2f(x + other.x, y + other.y);
    }

    Vector2f operator-(const Vector2f& other) const {
        return Vector2f(x - other.x, y - other.y);
    }
};

#endif // VECTOR2F_HPP
