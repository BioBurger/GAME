#include "Tile.h"

Tile::Tile(int x, int y, SDL_Texture *tex, int tileSize, bool animated, int frames, float speed, int Width, int Height) :
    texture(tex),
    currentFrameIndex(0),
    totalFrames(frames),
    animationSpeed(speed),
    animationTimer(0.0f),
    isAnimated(animated),
    frameWidth(Width),
    frameHeight(Height){

    position = {x, y, tileSize, tileSize};
    currentFrame = {0, 0, frameWidth, frameHeight};
}

void Tile::Update(float globalTime) {
    if (isAnimated) {
        int frameIndex = (static_cast<int>( globalTime / animationSpeed) % totalFrames);
        currentFrame.x = frameIndex * frameWidth;
    }
}



void Tile::Render(SDL_Renderer *renderer, const SDL_Rect &camera) {
    SDL_Rect dest = {
        position.x - camera.x,
        position.y - camera.y,
            position.w,
            position.h
    };
    SDL_RenderCopy(renderer, texture, &currentFrame, &dest);
}