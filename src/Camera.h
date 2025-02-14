#pragma once
#include <SDL.h>
#include <algorithm> // Za std::max/min

class Camera {
    SDL_Rect viewport; // x, y, w, h
    int mapWidth = INT_MAX; // Neskončen svet
    int mapHeight = INT_MAX;

public:
    Camera(int screenWidth, int screenHeight);
    void Update(int targetCenterX, int targetCenterY);
    SDL_Rect GetViewport() const { return viewport; }
    void SetMapBounds(int width, int height); //za stalno velikost
};