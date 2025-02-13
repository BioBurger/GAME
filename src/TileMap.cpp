#include "TileMap.h"

#include <map>

TileMap::TileMap(Texture_Manager &tm, Camera &cam) : textureManager(tm), camera(cam){}

void TileMap::Update() {
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
                int randtmp=rand()%(2);
                switch (randtmp) {
                    case 0:
                        textureID = "water1";
                    break;
                    case 1:
                        textureID = "water2";
                    break;
                }
                SDL_Texture *tex = textureManager.GetTexture(textureID);
                loadedTiles[key] = new Tile(x,y,tex,tileSize);
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
}

void TileMap::Render(SDL_Renderer* renderer) {
    for (auto& pair : loadedTiles) {
        pair.second->Render(renderer, camera.GetViewport());
    }
}

