#pragma once
#include <SDL.h>
#include <string>
#include <unordered_map>

class Texture_Manager {
    std::unordered_map<std::string, SDL_Texture*> textures;
    SDL_Renderer* renderer;
public:
    explicit Texture_Manager(SDL_Renderer* renderer);
    ~Texture_Manager();
    SDL_Texture* LoadTexture(const std::string& filePath, const std::string& texture_name);
    SDL_Texture* GetTexture(const std::string& texture_name);
    void UnloadTexture(const std::string& texture_name);
    void ClearTextures();
    bool HasTexture(const std::string& texture_name) const;
};