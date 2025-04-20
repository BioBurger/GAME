#pragma once
#include <vector>
#include <unordered_map>
#include "Tile.hpp"
#include "Camera.hpp"

class TileMap {
    std::unordered_map<std::string, Tile*> loadedTiles;// Tile
    Texture_Manager& textureManager;
    Camera& camera;
    float globalAnimationTimer = 0.0f;
    int tileSize = 64;// Size
    std::vector<std::vector<Tile*>> tileGrid;
public:
    TileMap(Texture_Manager& tm, Camera& cam);
    void Update(float deltaTime);
    void Render(SDL_Renderer* renderer, const SDL_Rect& cameraViewport);
};