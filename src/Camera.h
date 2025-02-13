#pragma once
#include <SDL.h>

class Camera {
    SDL_Rect viewport; // x, y, w, h (pozicija in velikost kamere)
    int mapWidth, mapHeight;
    float smoothSpeed = 1.0f;
    public:
    Camera(int screenWidth , int screenHeight);
    void Update(int targetX, int targetY);//sledenje
    SDL_Rect GetViewport() const {return viewport;}
    void WorldToScreen(int&x, int& y);
};