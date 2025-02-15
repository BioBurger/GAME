#pragma once
#include <SDL.h>
#include "Texture_Manager.hpp"

class Tile {
    SDL_Rect position;//world cordinate
    SDL_Texture* texture;
    SDL_Rect currentFrame;
    int frameWidth;
    int frameHeight;
    int currentFrameIndex;
    int totalFrames;
    float animationSpeed;
    float animationTimer;
    bool isAnimated;
public:
    Tile(int x, int y, SDL_Texture* tex,int tileSize, bool animated = false, int frames = 1, float speed = 0.0f, int Width = 16, int Height = 16);
    void Update(float globalTime);
    void Render(SDL_Renderer* renderer, const SDL_Rect& camera);
    SDL_Rect GetPosition() const {return position;}
};