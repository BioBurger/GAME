#ifndef VECTOR2F_HPP
#define VECTOR2F_HPP

#include <SDL2/SDL.h>
#include <cmath>

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
    static float Distance(const Vector2f& a, const Vector2f& b) {
        float dx = a.x - b.x;
        float dy = a.y - b.y;
        return std::sqrt(dx*dx + dy*dy);
    }
    float LengthSquared() const {
        return x * x + y * y;
    }
};

#endif // VECTOR2F_HPP
