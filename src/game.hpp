#include <iostream>
#include <ostream>
#include <stdio.h>
#include <SDL_image.h>
#include "GameObject.hpp"
#include "SDL2/SDL.h"
#include "Texture_Manager.hpp"
#include "Camera.hpp"
#include "TileMap.hpp"
#include "Projectile.hpp"
#include "Enemy.hpp"
#ifndef Game_h
#define Game_h
class Enemy;
class Projectile;

enum class GameState { MAIN_MENU, PLAYING, EXIT };

struct Collectible {
    SDL_Rect rect;
    bool active;
};

class Game {
private:
    GameState currentState = GameState::MAIN_MENU;
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
    std::vector<Projectile*> projectiles;
    float fireTimer = 0.0f;
    float FIRE_RATE = 1.0f;  // Shots per second
    float PISTOL_RANGE = 700.0f;
    const float PROJECTILE_SPEED = 800.0f;
    int PROJECTILE_DAMAGE = 50;
    int enemiesKilledThisWave = 0;
    bool isChoosingUpgrade = false;
    int selectedUpgrade = 0;
    SDL_Texture* upgradeMenuTexture = nullptr;
    SDL_Texture* upgradeFireTexture;
    SDL_Texture* upgradeDamageTexture;
    SDL_Texture* upgradeRangeTexture;
    SDL_Texture* menuBackground = nullptr;
    SDL_Texture* buttonTexture = nullptr;
    SDL_Texture* buttonHoverTexture = nullptr;
    std::vector<SDL_Rect> menuButtons;
    int hoveredButton = -1;
    const std::vector<std::pair<int, int>> resolutions = {
        {2560, 1440},  // 1440p
        {1920, 1080},  // 1080p
        {1280, 720}    // 720p
    };
    int windowWidth = 1920;
    int windowHeight = 1080;
    void UpdateButtonHover(int mouseX, int mouseY);
    void HandleMenuClick(int mouseX, int mouseY);
    void CreateMenuLayout();
    void ChangeResolution(int width, int height);
    int CalculateHeartPhase(int index, int currentHealth) const;
    int ScaleX(int original) const;
    int ScaleY(int original) const;
    const float WAVE_SCALE = 2.0f;
    const float NUMBER_SCALE = 2.0f;
    std::vector<Collectible> collectibles;
    float waveTimeRemaining;
    float waveTimeElapsed = 0.0f;
    SDL_Texture* collectibleTexture;
    const float WAVE_TIME_LIMIT = 30.0f;
    int collectiblesRemaining;
    SDL_Rect collectibleSrcRect;const int BASE_COLLECTIBLES = 3;
    const int COLLECTIBLES_PER_WAVE = 1;
    const int MIN_SPAWN_DISTANCE = 600;
    const int MAX_SPAWN_DISTANCE = 1000;


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
    Enemy* FindNearestEnemy();
    void ShootProjectile(Enemy* target);
    float DistanceToPlayer(Enemy* enemy);
    void ApplyUpgrade(int choice);
    void ReloadAllTextures();
    void SpawnCollectibles();
    void RenderTimer();
};

#endif //GAME_H