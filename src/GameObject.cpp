#include "GameObject.h"

GameObject::GameObject(Texture_Manager& manager, const std::string& texture_name , int x, int y, int w, int h, int frameWidth, int frameHeight, int totalFrames, float animationSpeed)
    : manager(manager),
        texture(manager.GetTexture(texture_name)),
        positionrect{x,y,w,h},velocityX(0),velocityY(0),positionX(static_cast<float>(x)),positionY(static_cast<float>(y)),
        frameWidth(frameWidth), frameHeight(frameHeight),
        currentFrameIndex(0), totalFrames(totalFrames),
        animationSpeed(animationSpeed), animationTimer(0.0f),
        isAnimated(totalFrames > 1) {
    texture = manager.GetTexture(texture_name);
    currentFrame = {0,0, frameWidth,frameHeight};
    if (!texture) {
        SDL_Log("TEXTURA '%s' NI NALOZENA", texture_name.c_str());
    }
}

GameObject::~GameObject() {}
void GameObject::Update(float deltaTime) {

    // Posodobi pozicijo
    positionX += velocityX * deltaTime;
    positionY += velocityY * deltaTime;

    // Pretvori v int za SDL_Rect
    positionrect.x = static_cast<int>(positionX);
    positionrect.y = static_cast<int>(positionY);

    if (isAnimated) {
        animationTimer += deltaTime;
        if (animationTimer >= animationSpeed) {
            currentFrameIndex = (currentFrameIndex + 1) % totalFrames;
            currentFrame.x = currentFrameIndex * frameWidth; //premik po sprite sheetu

            animationTimer = 0.0f;
        }
    }

}
void GameObject::Render(SDL_Renderer* renderer) {
    SDL_Rect destRect = {
    static_cast<int>(positionX),
        static_cast<int>(positionY),
        positionrect.w,
        positionrect.h
    };
    SDL_RenderCopy(renderer, texture, &currentFrame, &destRect);

}
void GameObject::SetPosition(int x, int y) {
    positionrect.x=x;
    positionrect.y=y;
}
void GameObject::SetVelocity(float vx, float vy) {
    velocityX = vx;
    velocityY = vy;
}
/*
#include "GameObject.h"

int main() {
// Inicializacija SDL, rendererja, itd.
SDL_Texture* playerTexture = ...; // Naloži teksturo z IMG_LoadTexture()

// Ustvari GameObject
GameObject player(playerTexture, 100, 100, 64, 64);
player.SetVelocity(200.0f, 0.0f); // Premikanje desno

// Glavna zanka
bool isRunning = true;
while (isRunning) {
float deltaTime = ...; // Izračunaj deltaTime

// Posodobi logiko
player.Update(deltaTime);

// Renderiranje
SDL_RenderClear(renderer);
player.Render(renderer);
SDL_RenderPresent(renderer);
}

return 0;
}*/

