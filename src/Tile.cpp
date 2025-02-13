#include "Tile.h"

Tile::Tile(int x, int y, SDL_Texture *tex): texture(tex) {
    position.x = x;
    position.y = y;
    position.w = 64;//tile širina/višina
    position.h = 64;
}

void Tile::Render(SDL_Renderer *renderer, const SDL_Rect &camera) {
    SDL_Rect dest =position;
    dest.x -= camera.x;//kamera kordinate
    dest.y -= camera.y;
    SDL_RenderCopy(renderer, texture,NULL, &dest);
}
