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
    const int PLAYER_SPAWN_X = 5000;
    const int PLAYER_SPAWN_Y = 5000;
    SDL_Texture* waveTextTexture = nullptr;
    SDL_Texture* numbersTexture = nullptr;
    const int NUMBER_WIDTH = 16;
    const int NUMBER_HEIGHT = 16;
    int currentWave = 0;
    int enemiesPerWave = 20;
    int enemiesRemaining = 0;
    float waveTimer = 0.0f;
    float waveStartDelay = 5.0f;
    bool betweenWaves = true;
    std::vector<Enemy*> enemyPool;
    SDL_Texture* cachedWaveTexture = nullptr;
    int cachedWaveNumber = -1;
    bool debugMode = false;
    const int SPAWN_OFFSET = 200;

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
    void RenderWaveNumber();
    void StartNewWave();
    Enemy* GetPooledEnemy();
    void InitializeEnemyPool(int initialSize);
    void ProcessInput();
    void UpdatePlayer(float deltaTime);
    void UpdateEnemies(float deltaTime);
    void UpdateWave(float deltaTime);
    SDL_Texture* CreateWaveNumberTexture();

};

#endif //GAME_H