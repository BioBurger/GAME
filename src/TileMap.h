#pragma once
#include <vector>
#include <unordered_map>
#include "Tile.h"
#include "Camera.h"

class TileMap {
    std::unordered_map<std::string, Tile*> loadedTiles;//naložen tile
    Texture_Manager& textureManager;
    Camera& camera;
    float globalAnimationTimer = 0.0f;
    int tileSize = 64;//velikost
public:
    TileMap(Texture_Manager& tm, Camera& cam);
    void Update(float deltaTime);
    void Render(SDL_Renderer* renderer, const SDL_Rect& cameraViewport);
};