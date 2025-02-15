#include <iostream>
#include <ostream>
#include <stdio.h>
#include <SDL_image.h>
#include "GameObject.hpp"
#include "SDL2/SDL.h"
#include "Texture_Manager.hpp"
#include "Camera.hpp"
#include "TileMap.hpp"
#ifndef Game_h
#define Game_h
class Enemy;

class Game {
private:
    Texture_Manager *texture_manager;
    bool isRunning;
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *heartTexture;
    GameObject *player;
    Camera *camera;
    TileMap *tileMap;
    std::vector<Enemy*> enemies;
    float spawnTimer = 0.0f;
    const float SPAWN_INTERVAL = 0.5f;
    bool gameOver = false;
    SDL_Texture* gameOverTexture;
    int playerWidth = 80;
    int playerHeight = 80;
    int frameWidth = 16;
    int frameHeight = 16;
    int totalFrames = 8;
    float animationSpeed = 0.1f;
    int srcHeartSize = 16;
    int destHeartSize = 64;
    int maxHearts = 4;
    static constexpr int HEART_PHASES = 7;
    static constexpr int HEALTH_PER_HEART = 25;
    const int PLAYER_SPAWN_X = 10000;
    const int PLAYER_SPAWN_Y = 10000;


public:
    Game();
    ~Game();
    void Init(const char* title, bool fullscreen);
    void HandleEvents();
    void Update(float deltaTime);
    void Render();
    void Clean();
    bool Running();
    void SpawnEnemy();
    bool CheckCollision(const SDL_Rect& a, const SDL_Rect& b);
    void RestartGame();
    bool IsGameOver() const { return gameOver; }

};

#endif //GAME_H