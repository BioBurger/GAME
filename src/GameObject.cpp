#include "GameObject.h"

GameObject::GameObject(Texture_Manager& manager, const std::string& texture_name , int x, int y, int w, int h)
    : manager(manager),
        texture(manager.GetTexture(texture_name)),
        positionrect{x,y,w,h},velocityX(0),velocityY() {
    if (!texture) {
        SDL_Log("TEXTURA '%s' NI NALOZENA", texture_name.c_str());
    }
}

GameObject::~GameObject() {}
void GameObject::Update(float Time) {
    positionrect.x += static_cast<int>(velocityX*Time);
    positionrect.y += static_cast<int>(velocityY*Time);
}
void GameObject::Render(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, texture, NULL, &positionrect);
}
void GameObject::SetPosition(int x, int y) {
    positionrect.x=x;
    positionrect.y=y;
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

