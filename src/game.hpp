#include <iostream>
#include <ostream>
#include <cstdint>
#include <ctime>
#include <stdio.h>
#include <SDL_image.h>
#include <shlobj.h>
#include <fstream>
#include "GameObject.hpp"
#include "SDL2/SDL.h"
#include "Texture_Manager.hpp"
#include "Camera.hpp"
#include "TileMap.hpp"
#include "Projectile.hpp"
#include "Enemy.hpp"
#include "Ally.hpp"
#ifndef Game_h
#define Game_h
class Enemy;
class Projectile;
class Ally;

enum class GameState { MAIN_MENU, SETTINGS, PLAYING, EXIT, HIGHSCORE_ENTRY, HIGHSCORES_DISPLAY, GAME_OVER, REPLAY };

struct Collectible {
    SDL_Rect rect;
    bool active;
};
struct MenuButton {
    std::string id;
    SDL_Rect rect;
};
struct HighScore {
    std::string name;
    int score;
};
#pragma pack(push, 1)
struct ReplayHeader {
    char magic[4];
    uint32_t frameCount;
    float totalTime;
    int finalScore;
    int finalWave;
};

struct ReplayFrame {
    float timestamp;
    Vector2f playerPos;
    Vector2f playerVelocity; // ADD
    int playerFrameIndex;    // ADD
    uint16_t enemyCount;
    uint16_t projectileCount;
    uint16_t collectibleCount;
    Vector2f allyPos;
    Vector2f allyVelocity;   // ADD
    int allyFrameIndex;      // ADD
    bool allyActive;
};
#pragma pack(pop)

class Game {
private:

    GameState currentState = GameState::MAIN_MENU;
    Texture_Manager *texture_manager;
    bool isRunning;
    SDL_Window *window;
    SDL_Renderer *renderer;
    bool gameOver = false;
    SDL_Texture* gameOverTexture;
    bool debugMode = false;

    //Player
    GameObject *player;
    int playerWidth = 80;
    int playerHeight = 80;
    int frameWidth = 16;
    int frameHeight = 16;
    int totalFrames = 8;
    float animationSpeed = 0.1f;
    const int PLAYER_SPAWN_X = 5000;
    const int PLAYER_SPAWN_Y = 5000;
    //Camera
    Camera *camera;
    //Map
    TileMap *tileMap;

    //Enemies
    std::vector<Enemy*> enemies;
    float spawnTimer = 0.0f;
    const float SPAWN_INTERVAL = 0.5f;
    int enemiesPerWave = 20;
    int enemiesRemaining = 0;
    std::vector<Enemy*> enemyPool;
    int enemiesKilledThisWave = 0;

    //Health
    SDL_Texture *heartTexture;
    int srcHeartSize = 16;
    int destHeartSize = 64;
    int maxHearts = 4;
    static constexpr int HEART_PHASES = 7;
    static constexpr int HEALTH_PER_HEART = 25;
    int CalculateHeartPhase(int index, int currentHealth) const;

    //Numbers+Text+Wave
    SDL_Texture* waveTextTexture = nullptr;
    SDL_Texture* numbersTexture = nullptr;
    SDL_Texture* cachedWaveTexture = nullptr;
    const int NUMBER_WIDTH = 16;
    const int NUMBER_HEIGHT = 16;
    int currentWave = 0;
    float waveTimer = 0.0f;
    float waveStartDelay = 5.0f;
    bool betweenWaves = true;
    int cachedWaveNumber = -1;
    const float WAVE_SCALE = 2.0f;
    const float NUMBER_SCALE = 2.0f;
    float waveTimeRemaining;
    float waveTimeElapsed = 0.0f;
    const float WAVE_TIME_LIMIT = 30.0f;


    //Ally
    Ally* ally;
    const int SPAWN_OFFSET = 200;

    //Projectiles
    std::vector<Projectile*> projectiles;
    float fireTimer = 0.0f;
    float FIRE_RATE = 1.0f;  // Shots per second
    float PISTOL_RANGE = 700.0f;
    const float PROJECTILE_SPEED = 800.0f;
    int PROJECTILE_DAMAGE = 50;

    //Upgrade
    bool isChoosingUpgrade = false;
    int selectedUpgrade = 0;
    SDL_Texture* upgradeMenuTexture = nullptr;
    SDL_Texture* upgradeFireTexture;
    SDL_Texture* upgradeDamageTexture;
    SDL_Texture* upgradeRangeTexture;

    //Menu
    SDL_Texture* lettersTexture;
    SDL_Rect letterSrcRect;
    SDL_Texture* menuBackground = nullptr;
    SDL_Texture* buttonTexture = nullptr;
    SDL_Texture* buttonHoverTexture = nullptr;
    std::vector<MenuButton> menuButtons;
    int hoveredButton = -1;
    std::vector<std::pair<int, int>> resolutions = {
        {1280, 720},
        {1920, 1080},
        {2560, 1440}
    };
    int windowWidth = 1920;
    int windowHeight = 1080;
    void UpdateButtonHover(int mouseX, int mouseY);
    void HandleMenuClick(int mouseX, int mouseY);
    void CreateMenuLayout();
    void ChangeResolution(int width, int height);
    int ScaleX(int original) const;
    int ScaleY(int original) const;
    const int LETTER_WIDTH = 16;
    const int LETTER_HEIGHT = 16;
    const int MAX_NAME_LENGTH = 3;
    int totalScore=0;
    int enemiesKilledTotal=0;
    int collectiblesCollected=0;

    //Collectibles
    SDL_Texture* collectibleTexture;
    std::vector<Collectible> collectibles;
    int collectiblesRemaining;
    SDL_Rect collectibleSrcRect;const int BASE_COLLECTIBLES = 3;
    const int COLLECTIBLES_PER_WAVE = 1;
    const int MIN_SPAWN_DISTANCE = 600;
    const int MAX_SPAWN_DISTANCE = 1000;



    std::vector<HighScore> highScores;
    std::string currentNameInput;
    bool isEnteringName;


    //Load+Replay
    std::string GetSaveDirectory();
    std::string GetSavePath(const std::string& filename);
    void CreateGameDirectories();
    std::string currentReplayFile;
    float replayTimer;
    float replaySpeed = 1.0f;
    std::ifstream replayFile;
    uint32_t totalFramesRecorded;
    uint32_t currentReplayFrame;
    void StartRecording();
    void RecordFrame(float deltaTime);
    void FinalizeRecording();
    void UpdateReplay(float deltaTime);
    bool isRecording = false;
    bool isReplaying = false;
    float totalRecordedTime = 0.0f;
    void HandleGameOverInput(const SDL_Event& event);
    float recordingFrameTimer = 0.0f;
    const float RECORD_INTERVAL = 1.0f / 60.0f;

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
    void StartNewWave(int wave = -1);
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
    Enemy* FindNearestEnemyToAlly();
    void ShootAllyProjectile(Enemy* target);
    void SaveGame();
    bool LoadGame();
    static std::string GetSavePath() {
        char path[MAX_PATH];
        if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, path))) {
            std::string fullPath = std::string(path) + "\\GAME\\";
            if (!CreateDirectoryA(fullPath.c_str(), NULL) && (GetLastError() != ERROR_ALREADY_EXISTS)) {
                return "savegame.dat";
            }
            return fullPath + "savegame.dat";
        }
        return "savegame.dat";
    }
    void SaveHighScores();
    void LoadHighScores();
    void RenderText(const std::string& text, int x, int y, float scale = 1.0f);
    void HandleHighscoreInput(const SDL_Event& event);
    void RenderNumber(int number, int, int, float);
    void StartReplay(const std::string& filename);
};

#endif //GAME_H