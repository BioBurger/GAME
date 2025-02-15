#include "Texture_Manager.hpp"
#include <SDL_image.h>



Texture_Manager::Texture_Manager(SDL_Renderer *renderer): renderer(renderer) {}
Texture_Manager::~Texture_Manager() {
    ClearTextures();
}
SDL_Texture* Texture_Manager::LoadTexture(const std::string& filePath, const std::string& file_name) {
        auto iterator= textures.find(file_name);
        if (iterator != textures.end()) {
            return iterator->second;
        }

       SDL_Surface* surface = IMG_Load(filePath.c_str());
        if (surface == NULL) {
            SDL_Log("NAPAKA PRI LOADANJU SLIKE IZ %s: %s", filePath.c_str(), SDL_GetError());
            return NULL;
        }
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        if (texture == NULL) {
            SDL_Log("NAPAKA PRI USTVARJANJU TEXTURE IZ %s: %s", filePath.c_str(), SDL_GetError());
            return NULL;
        }
        textures[file_name] = texture;
        return texture;
    }
SDL_Texture* Texture_Manager::GetTexture(const std::string& file_name) {
        auto iterator = textures.find(file_name);
        return (iterator != textures.end()) ? iterator->second : NULL;
    }
void Texture_Manager::UnloadTexture(const std::string& file_name) {
        auto iterator = textures.find(file_name);
        if (iterator != textures.end()) {
            SDL_DestroyTexture(iterator->second);
            textures.erase(iterator);
        }
    }
void Texture_Manager::ClearTextures() {
        for (auto& pair : textures) {
            SDL_DestroyTexture(pair.second);
        }
        textures.clear();
}
bool Texture_Manager::HasTexture(const std::string& texture_name) const {
    return textures.find(texture_name) != textures.end();
}
