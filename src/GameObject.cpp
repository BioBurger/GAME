#include "GameObject.h"

GameObject::GameObject(Texture_Manager& manager, const std::string& texture_name , int x, int y, int w, int h)
    : manager(manager),
        texture(manager.GetTexture(texture_name)),
        positionrect{x,y,w,h},velocityX(0),velocityY(0),positionX(static_cast<float>(x)),positionY(static_cast<float>(y)) {
    if (!texture) {
        SDL_Log("TEXTURA '%s' NI NALOZENA", texture_name.c_str());
    }
}

GameObject::~GameObject() {}
void GameObject::Update(float deltaTime) {
    // Debugging izpis (preveri hitrost in deltaTime)
    SDL_Log("Premik: velocityX=%.2f, velocityY=%.2f, deltaTime=%.4f", velocityX, velocityY, deltaTime);

    // Posodobi pozicijo s plavajočo vejico
    positionX += velocityX * deltaTime;
    positionY += velocityY * deltaTime;

    // Pretvori v int za SDL_Rect
    positionrect.x = static_cast<int>(positionX);
    positionrect.y = static_cast<int>(positionY);

    // Debugging izpis (preveri končno pozicijo)
    SDL_Log("Nova pozicija: posX=%.2f, posY=%.2f, rect.x=%d, rect.y=%d",
            positionX, positionY, positionrect.x, positionrect.y);
}
void GameObject::Render(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, texture, NULL, &positionrect);
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

