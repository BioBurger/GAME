#include "TileMap.hpp"

#include <map>

TileMap::TileMap(Texture_Manager &tm, Camera &cam) : textureManager(tm), camera(cam){}

void TileMap::Update(float deltaTime) {
    globalAnimationTimer += deltaTime;//posodobim globalni čas
    //V oknu
    SDL_Rect viewport = camera.GetViewport();
    int startX = (viewport.x / tileSize) * tileSize;
    int startY = (viewport.y / tileSize) * tileSize;
    int endX = startX + viewport.w + tileSize * 2;
    int endY = startY + viewport.h + tileSize * 2;

    //Load tile
    for (int y = startY; y < endY; y+=tileSize) {
        for (int x = startX; x < endX; x+=tileSize) {
            std::string key = std::to_string(x) + "," + std::to_string(y);

            if (loadedTiles.find(key) == loadedTiles.end()) {
                //loadanje sproti
                std::string textureID;

                int randtmp=0;
                switch (randtmp) {
                    case 0:
                        textureID = "water";
                    break;
                }

                bool isAnimated = (textureID == "water");
                int frames = 3;
                float speed = 0.5f;
                int frameWidth = 16;
                int frameHeight = 16;

                SDL_Texture *tex = textureManager.GetTexture(textureID);
                loadedTiles[key] = new Tile(x,y,tex,tileSize, isAnimated, frames, speed, frameWidth, frameHeight);
            }
        }
    }
    //Božji fix da komp ne umre(sprotno brisanje)
    auto it = loadedTiles.begin();
    while (it != loadedTiles.end()) {
        SDL_Rect tilePos = it->second->GetPosition();
        int tileX = tilePos.x;
        int tileY = tilePos.y;
        if (tileX < startX - tileSize || tileX > endX || tileY < startY - tileSize || tileY > endY) {
            delete it->second;
            it = loadedTiles.erase(it);
        }else {
            ++it;
        }
    }
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