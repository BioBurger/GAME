#pragma once
#include <SDL.h>
#include <algorithm>

class Camera {
    SDL_Rect viewport;  // viewport.x in .y se nastavita iz float spremenljivk
    float x, y;         // Decimalke za smoothh
    int mapWidth = INT_MAX;
    int mapHeight = INT_MAX;

public:
    Camera(int screenWidth, int screenHeight);
    void Update(int targetCenterX, int targetCenterY); // Se da float
    SDL_Rect GetViewport() const { return viewport; }
    void SetMapBounds(int width, int height);
    void Reset(int targetCenterX, int targetCenterY);
};