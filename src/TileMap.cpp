#include "TileMap.hpp"

#include <map>

TileMap::TileMap(Texture_Manager &tm, Camera &cam) : textureManager(tm), camera(cam){}

void TileMap::Update(float deltaTime) {
    globalAnimationTimer += deltaTime;
    SDL_Rect viewport = camera.GetViewport();

    // Določi območje tile-ov glede na kamero v gridu
    int startCol = (viewport.x / tileSize) - 1;
    int endCol = (viewport.x + viewport.w) / tileSize + 1;
    int startRow = (viewport.y / tileSize) - 1;
    int endRow = (viewport.y + viewport.h) / tileSize + 1;

    // Naloži nove tile-e v vidnem območju
    for (int row = startRow; row <= endRow; ++row) {
        for (int col = startCol; col <= endCol; ++col) {
            int x = col * tileSize;
            int y = row * tileSize;
            std::string key = std::to_string(x) + "," + std::to_string(y);

            if (loadedTiles.find(key) == loadedTiles.end()) {
                std::string textureID = "water";
                bool isAnimated = true;
                int frames = 3;
                float speed = 0.5f;

                SDL_Texture* tex = textureManager.GetTexture(textureID);
                loadedTiles[key] = new Tile(x, y, tex, tileSize, isAnimated, frames, speed, 16, 16);
            }
        }
    }

    // Odstrani tile-e izven vidnega območja
    auto it = loadedTiles.begin();
    while (it != loadedTiles.end()) {
        SDL_Rect pos = it->second->GetPosition();
        int tileCol = pos.x / tileSize;
        int tileRow = pos.y / tileSize;

        if (tileCol < startCol - 1 || tileCol > endCol + 1 ||
            tileRow < startRow - 1 || tileRow > endRow + 1) {
            delete it->second;
            it = loadedTiles.erase(it);
            } else {
                ++it;
            }
    }

    // Posodobi animacije za vse tile-e
    for (auto& pair : loadedTiles) {
        pair.second->Update(globalAnimationTimer);
    }
}

void TileMap::Render(SDL_Renderer* renderer, const SDL_Rect& cameraViewport) {
    for (auto& pair : loadedTiles) {
        SDL_Rect tilePos = pair.second->GetPosition();

        // Preveri če je tile viden
        if (tilePos.x + tilePos.w < cameraViewport.x ||
            tilePos.x > cameraViewport.x + cameraViewport.w ||
            tilePos.y + tilePos.h < cameraViewport.y ||
            tilePos.y > cameraViewport.y + cameraViewport.h) {
            continue;
            }

        pair.second->Render(renderer, cameraViewport);
    }
}