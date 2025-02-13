#pragma once
#include <vector>
#include <unordered_map>
#include "Tile.h"
#include "Camera.h"

class TileMap {
    std::unordered_map<std::string, Tile*> loadedTiles;//naložen tile
    Texture_Manager& textureManager;
    Camera& camera;
    int tileSize = 64;//velikost
    public:
    TileMap(Texture_Manager& tm, Camera& cam);
    void Update();
    void Render(SDL_Renderer* renderer);
};