#pragma once
#include <SDL.h>
#include <algorithm>

class Camera {
    SDL_Rect viewport;
    float x, y;         // Decimalke za smoothh
    int mapWidth = INT_MAX;
    int mapHeight = INT_MAX;

public:
    Camera(int screenWidth, int screenHeight);
    void Update(int targetCenterX, int targetCenterY); // Se da float
    SDL_Rect GetViewport() const { return viewport; }
    void SetMapBounds(int width, int height);
    void Reset(int targetCenterX, int targetCenterY);
    void UpdateDimensions(int width, int height) {
        viewport.w = width;
        viewport.h = height;
    }
};