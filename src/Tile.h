#pragma once
#include <SDL.h>
#include "Texture_Manager.h"

class Tile {
    SDL_Rect position;//world cordinate
    SDL_Texture* texture;
    public:
    Tile(int x, int y, SDL_Texture* tex,int tileSize);
    void Render(SDL_Renderer* renderer, const SDL_Rect& camera);
    SDL_Rect GetPosition() const {return position;}
};
