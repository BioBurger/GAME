#pragma once
#include <SDL.h>
#include <algorithm>
#include "GameObject.hpp"

class Camera {
    SDL_Rect viewport;
    float x, y;         // Decimalke za smoothh
    int mapWidth = INT_MAX;
    int mapHeight = INT_MAX;
    GameObject* target = nullptr;

public:
    Camera(int screenWidth, int screenHeight);
    void Update(float targetCenterX, float targetCenterY);
    SDL_Rect GetViewport() const { return viewport; }
    void SetMapBounds(int width, int height);
    void Reset(int targetCenterX, int targetCenterY);
    void SetTarget(GameObject* newTarget);
    void UpdateDimensions(int width, int height) {
        viewport.w = width;
        viewport.h = height;
    }
};