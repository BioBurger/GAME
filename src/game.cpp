#include "Game.hpp"
#include <SDL2/SDL_keyboard.h>
#include <SDL_filesystem.h>
#include <filesystem>
#include <random>
#include "Projectile.hpp"
#include "Enemy.hpp"

Game::Game() : texture_manager(NULL), isRecording(false), isReplaying(false)  {
    texture_manager=NULL;
    player=NULL;
}

Game::~Game() {
    for (auto& e : enemyPool) {
        delete e;
    }
    enemyPool.clear();
    delete player;
    delete texture_manager;
    delete camera;
    delete tileMap;
    delete ally;
    for (auto& p : projectiles) {
        delete p;
    }
    projectiles.clear();
    SDL_DestroyTexture(waveTextTexture);
    SDL_DestroyTexture(numbersTexture);

};
std::string Game::GetSavePath(const std::string& filename) {
    return GetSaveDirectory() + filename;
}
void Game::CreateGameDirectories() {
    std::filesystem::create_directories(GetSaveDirectory());
}

std::string Game::GetSaveDirectory() {
    char* basePath = SDL_GetBasePath();
    std::string path(basePath);
    SDL_free(basePath);
    path += "GAME/saves/";
    return path;
}

void Game::Init(const char* title, bool fullscreen) {
    int flags = 0;
    if (fullscreen) {
        flags = SDL_WINDOW_FULLSCREEN;
    }

    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920, 1080, flags);
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

        if (renderer) {
            // Start
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            SDL_RenderPresent(renderer);
            texture_manager = new Texture_Manager(renderer);

            // Menu
            if (!texture_manager->HasTexture("menu_bg")) {
                if (!texture_manager->LoadTexture("assets/textures/menu/background.png", "menu_bg")) {
                    SDL_Log("No menu texture!");
                    exit(1);
                }
            }
            if (!texture_manager->HasTexture("play_btn")) {
                if (!texture_manager->LoadTexture("assets/textures/menu/play_btn.png", "play_btn")) {
                    SDL_Log("No menu texture!");
                    exit(1);
                }
            }
            if (!texture_manager->HasTexture("play_btn_hover")) {
                if (!texture_manager->LoadTexture("assets/textures/menu/play_btn_hover.png", "play_btn_hover")) {
                    SDL_Log("No menu texture!");
                    exit(1);
                }
            }
            if (!texture_manager->HasTexture("settings_btn")) {
                if (!texture_manager->LoadTexture("assets/textures/menu/settings_btn.png", "settings_btn")) {
                    SDL_Log("No menu texture!");
                    exit(1);
                }
            }
            if (!texture_manager->HasTexture("load_btn")) {
                if (!texture_manager->LoadTexture("assets/textures/menu/load_btn.png", "load_btn")) {
                    SDL_Log("No menu texture!");
                }
            }
            if (!texture_manager->HasTexture("load_btn_hover")) {
                if (!texture_manager->LoadTexture("assets/textures/menu/load_btn_hover.png", "load_btn_hover")) {
                    SDL_Log("No menu texture!");
                }
            }
            if (!texture_manager->HasTexture("settings_btn_hove")) {
                if (!texture_manager->LoadTexture("assets/textures/menu/settings_btn_hover.png", "settings_btn_hover")) {
                    SDL_Log("No menu texture!");
                    exit(1);
                }
            }
            if (!texture_manager->HasTexture("quit_btn")) {
                if (!texture_manager->LoadTexture("assets/textures/menu/quit_btn.png", "quit_btn")) {
                    SDL_Log("No menu texture!");
                    exit(1);
                }
            }
            if (!texture_manager->HasTexture("quit_btn_hover")) {
                if (!texture_manager->LoadTexture("assets/textures/menu/quit_btn_hover.png", "quit_btn_hover")) {
                    SDL_Log("No menu texture!");
                    exit(1);
                }
            }
            if (!texture_manager->HasTexture("scores_btn")) {
                if (!texture_manager->LoadTexture("assets/textures/menu/scores_btn.png", "scores_btn")) {
                    SDL_Log("No menu texture!");
                    exit(1);
                }
            }
            if (!texture_manager->HasTexture("scores_btn_hover")) {
                if (!texture_manager->LoadTexture("assets/textures/menu/scores_btn_hover.png", "scores_btn_hover")) {
                    SDL_Log("No menu texture!");
                    exit(1);
                }
            }
            if (!texture_manager->HasTexture("back_btn")) {
                if (!texture_manager->LoadTexture("assets/textures/menu/back_btn.png", "back_btn")) {
                    SDL_Log("No menu texture!");
                    exit(1);
                }
            }
            if (!texture_manager->HasTexture("back_btn_hover")) {
                if (!texture_manager->LoadTexture("assets/textures/menu/back_btn_hover.png", "back_btn_hover")) {
                    SDL_Log("No menu texture!");
                    exit(1);
                }
            }
            if (!texture_manager->HasTexture("letters")) {
                if(!texture_manager->LoadTexture("assets/textures/ui/letters.png", "letters")) {
                    SDL_Log("Letters texture!");
                }
            }
            lettersTexture = texture_manager->GetTexture("letters");
            letterSrcRect = {0, 0, LETTER_WIDTH, LETTER_HEIGHT};

            LoadHighScores();

            std::vector<std::pair<int, int>> resolutions = {
                {1280, 720},
                {1920, 1080},
                {2560, 1440}
            };
            for (const auto& res : resolutions) {
                std::string resStr = std::to_string(res.first) + "x" + std::to_string(res.second);

                // Normal
                std::string normalName = "resolution_btn_" + resStr;
                if (!texture_manager->LoadTexture(("assets/textures/menu/" + normalName + ".png").c_str(), normalName)) {
                    SDL_Log("FAILED TO LOAD: %s", normalName.c_str());
                    exit(1);
                }

                // Hover
                std::string hoverName = "resolution_btn_hover_" + resStr;
                if (!texture_manager->LoadTexture(("assets/textures/menu/" + hoverName + ".png").c_str(), hoverName)) {
                    SDL_Log("FAILED TO LOAD: %s", hoverName.c_str());
                    exit(1);
                }
            }
            // Player
            if (!texture_manager->HasTexture("player")) {
                if(!texture_manager->LoadTexture("assets/textures/Player/Turtle-Wick.png", "player")) {
                    SDL_Log("No player texture");
                    exit(1);
                }
            }
            // Ally
            if (!texture_manager->HasTexture("ally")) {
                if(!texture_manager->LoadTexture("assets/textures/Player/ally.png", "ally")) {
                    SDL_Log("No ally texture!");
                    exit(1);
                }
            }

            // Water
            if (!texture_manager->HasTexture("water")) {
                if(!texture_manager->LoadTexture("assets/textures/Tiles/water_sheet.png", "water")) {
                    SDL_Log("No water texture!");
                    exit(1);
                }

            }
            // Enemys
            if (!texture_manager->HasTexture("enemy1")) {
                if (!texture_manager->LoadTexture("assets/textures/Enemys/test.png", "enemy1")) {
                    SDL_Log("No enemy texture!");
                    exit(1);
                }
            }
            if (!texture_manager->HasTexture("enemy2")) {
                if (!texture_manager->LoadTexture("assets/textures/Enemys/newenemy2.png", "enemy2")) {
                    SDL_Log("No enemy texture!");
                    exit(1);
                }
            }
            if (!texture_manager->HasTexture("enemy3")) {
                if (!texture_manager->LoadTexture("assets/textures/Enemys/newenemy3.png", "enemy3")) {
                    SDL_Log("No enemy texture!");
                    exit(1);
                }
            }
            // Game Over
            if (!texture_manager->HasTexture("game_over")) {
                if(!texture_manager->LoadTexture("assets/textures/ui/GameOver.png", "game_over")) {
                    SDL_Log("No game over texture!");
                    exit(1);
                }
            }
            // UI elementi
            if (!texture_manager->HasTexture("heart")) {
                if(!texture_manager->LoadTexture("assets/textures/ui/Heart.png", "heart")) {
                    SDL_Log("No heart texture!");
                    exit(1);
                }
            }// Bullet
            if (!texture_manager->HasTexture("bullet")) {
                if (!texture_manager->LoadTexture("assets/textures/Weapons/bullet.png", "bullet")) {
                    SDL_Log("No bullet texture!");
                    exit(1);
                }
            }
            // Števile za wave
            if (!texture_manager->HasTexture("numbers")) {
                if(!texture_manager->LoadTexture("assets/textures/ui/numbers.png", "numbers")) {
                    SDL_Log("No numbers texture!");
                    exit(1);
                }
            }
            // Wave napis
            if (!texture_manager->HasTexture("wave_text")) {
                if(!texture_manager->LoadTexture("assets/textures/ui/wave.png", "wave_text")) {
                    SDL_Log("No wave texture!");
                    exit(1);
                }
            }
            if (!texture_manager->HasTexture("collectible")) {
                if(!texture_manager->LoadTexture("assets/textures/collectible2.png", "collectible")) {
                    SDL_Log("No collectible texture!");
                    exit(1);
                }
            }
            // Upgradi
            if (!texture_manager->HasTexture("upgrade_menu")) {
                if(!texture_manager->LoadTexture("assets/textures/ui/upgrade_menu.png", "upgrade_menu")) {
                    SDL_Log("No upgrade_menu texture!");
                    exit(1);
                }

            }
            if (!texture_manager->HasTexture("upgrade_fire_rate")) {
                if(!texture_manager->LoadTexture("assets/textures/ui/upgrade_fire.png", "upgrade_fire_rate")) {
                    SDL_Log("No upgrade_fire texture!");
                    exit(1);
                }

            }
            if (!texture_manager->HasTexture("upgrade_damage")) {
                if(!texture_manager->LoadTexture("assets/textures/ui/upgrade_damage.png", "upgrade_damage")) {
                    SDL_Log("No upgrade_damage texture!");
                    exit(1);
                }

            }
            if (!texture_manager->HasTexture("upgrade_range")) {
                if(!texture_manager->LoadTexture("assets/textures/ui/upgrade_range.png", "upgrade_range")) {
                    SDL_Log("No upgrade_range texture!");
                    exit(1);
                }

            }
            if (!texture_manager->HasTexture("arrow")) {
                if(!texture_manager->LoadTexture("assets/textures/ui/arrow.png", "arrow")) {
                    SDL_Log("No arrow texture!");
                    exit(1);
                }

            }
            // Reference v mapi
            upgradeMenuTexture = texture_manager->GetTexture("upgrade_menu");
            heartTexture = texture_manager->GetTexture("heart");
            numbersTexture = texture_manager->GetTexture("numbers");
            waveTextTexture = texture_manager->GetTexture("wave_text");
            gameOverTexture = texture_manager->GetTexture("game_over");
            upgradeFireTexture = texture_manager->GetTexture("upgrade_fire_rate");
            upgradeDamageTexture = texture_manager->GetTexture("upgrade_damage");
            upgradeRangeTexture = texture_manager->GetTexture("upgrade_range");
            menuBackground = texture_manager->GetTexture("menu_bg");
            buttonTexture = texture_manager->GetTexture("button");
            buttonHoverTexture = texture_manager->GetTexture("button_hover");
            collectibleTexture = texture_manager->GetTexture("collectible");

            collectibleSrcRect = {0, 0, 64, 64};

            SDL_SetWindowSize(window, windowWidth, windowHeight);
            SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

            betweenWaves = true;
            waveTimer = 0.0f;
            currentWave = 0;


            // Player declare
            player = new GameObject(*texture_manager,"player",PLAYER_SPAWN_X, PLAYER_SPAWN_Y,playerWidth, playerHeight,frameWidth, frameHeight,totalFrames, animationSpeed);
            ally = new Ally(*texture_manager, "ally", PLAYER_SPAWN_X + 100, PLAYER_SPAWN_Y);


            camera = new Camera(1920, 1080);
            camera->Reset(PLAYER_SPAWN_X + playerWidth/2, PLAYER_SPAWN_Y + playerHeight/2);
            camera->SetMapBounds(20000, 20000);
            tileMap = new TileMap(*texture_manager, *camera);

            InitializeEnemyPool(20);//Enemys per wave at 1
            isRunning = true;
            CreateMenuLayout();
            SDL_SetWindowSize(window, windowWidth, windowHeight);
            SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
        }
    } else {
        isRunning = false;
        SDL_Log("NAPACNA INICIALIZACIJA SDL: %s", SDL_GetError());
    }

}


void Game::HandleEvents() {
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        switch(event.type) {
            case SDL_QUIT:
                isRunning = false;
                break;

            case SDL_MOUSEMOTION:
                UpdateButtonHover(event.motion.x, event.motion.y);
                break;

            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    HandleMenuClick(event.button.x, event.button.y);
                }
                break;
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    windowWidth = event.window.data1;
                    windowHeight = event.window.data2;

                    CreateMenuLayout();
                }
            break;
            case SDL_KEYDOWN:
                if (currentState == GameState::GAME_OVER) {
                    HandleGameOverInput(event);
                    break;
                }
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    if (currentState == GameState::PLAYING ||
                        currentState == GameState::SETTINGS ||
                        currentState == GameState::HIGHSCORES_DISPLAY) {
                        currentState = GameState::MAIN_MENU;
                        SaveGame();
                        CreateMenuLayout();
                        }
                }

                if (currentState == GameState::HIGHSCORE_ENTRY) {
                    HandleHighscoreInput(event);
                    break;
                }
                if (isChoosingUpgrade) {
                    switch(event.key.keysym.sym) {
                        case SDLK_LEFT:
                            selectedUpgrade = (selectedUpgrade - 1 + 3) % 3;
                            break;
                        case SDLK_RIGHT:
                            selectedUpgrade = (selectedUpgrade + 1) % 3;
                            break;
                        case SDLK_RETURN:
                            ApplyUpgrade(selectedUpgrade);
                            isChoosingUpgrade = false;
                            break;
                    }
                }

                switch(event.key.keysym.sym) {
                    case SDLK_F1:
                        debugMode = !debugMode;
                        break;
                    case SDLK_F2:
                        StartNewWave();
                        break;
                    case SDLK_F5:
                        if (currentState == GameState::PLAYING) {
                            SaveGame();
                            SDL_Log("Game saved successfully!");
                        }
                        break;
                    case SDLK_F6:
                        if (currentState == GameState::MAIN_MENU || currentState == GameState::PLAYING) {
                            if (LoadGame()) {
                                SDL_Log("Game loaded successfully!");
                                if (currentState == GameState::MAIN_MENU) {
                                    currentState = GameState::PLAYING;
                                }
                            }
                        }
                        break;
                    case SDLK_KP_PLUS:
                        player->SetSpeedMultiplier(1.5f);
                        break;
                    case SDLK_KP_MINUS:
                        player->SetSpeedMultiplier(0.75f);
                        break;
                }
                break;
        }
    }
}
void Game::HandleGameOverInput(const SDL_Event& event) {
    if (event.type == SDL_KEYDOWN) {
        switch(event.key.keysym.sym) {
            case SDLK_r:
                RestartGame();
                currentState = GameState::PLAYING;
                break;
            case SDLK_h:
                currentState = GameState::HIGHSCORE_ENTRY;
                currentNameInput.clear();
                break;
            case SDLK_p:
                StartReplay(GetSavePath("last_replay.bin"));
                break;
        }
    }
}

void Game::Update(float deltaTime) {
    if(currentState == GameState::REPLAY) {
        UpdateReplay(deltaTime);
        return;
    }

    if (isRecording && currentState == GameState::PLAYING) {
        recordingFrameTimer += deltaTime;
        while (recordingFrameTimer >= RECORD_INTERVAL) {
            RecordFrame(RECORD_INTERVAL);  // JUst 60fps
            recordingFrameTimer -= RECORD_INTERVAL;
        }
    }
    if (currentState != GameState::PLAYING) {
        return;
    }

    // If gameover
    if ((!player->IsAlive() || waveTimeRemaining <= 0) && !gameOver) {
        gameOver = true;
        currentState = GameState::GAME_OVER;
        return;
    }

    if (gameOver || !player->IsAlive()) return;
    if (isChoosingUpgrade) return;

    // Update damage cooldown
    if (player->GetDamageCooldown() > 0.0f) {
        player->SetDamageCooldown(player->GetDamageCooldown() - deltaTime);
    }

    // Ally
    if (ally && ally->IsAlive()) {
        const Vector2f targetPos = player->GetCenterPosition() + Vector2f(50, 50);
        const Vector2f allyPos = ally->GetCenterPosition();
        const Vector2f direction = targetPos - allyPos;
        const float distance = direction.x * direction.x + direction.y * direction.y;

        if (distance > 100.0f) {
            const float speed = 200.0f;
            const float invDistance = 1.0f / std::sqrt(distance);
            ally->SetVelocity(direction.x * invDistance * speed, direction.y * invDistance * speed);
        } else {
            ally->SetVelocity(0, 0);
        }

        static float fireTimer = 0.0f;
        fireTimer += deltaTime;

        if (Enemy* nearest = FindNearestEnemyToAlly()) {
            const float attackRange = 500.0f;
            const float sqrRange = attackRange * attackRange;
            const Vector2f enemyPos = nearest->GetCenterPosition();

            if ((enemyPos - allyPos).LengthSquared() <= sqrRange &&
                fireTimer >= 1.0f/1.5f) {
                ShootAllyProjectile(nearest);
                fireTimer = 0.0f;
            }
        }
        ally->Update(deltaTime);
    }

    // Updates
    UpdateWave(deltaTime);
    ProcessInput();
    UpdatePlayer(deltaTime);
    UpdateEnemies(deltaTime);

    // Projectile updates
    for (auto it = projectiles.begin(); it != projectiles.end();) {
        (*it)->Update(deltaTime);
        if (CheckCollision((*it)->GetCollisionBox(), (*it)->GetTarget()->GetCollisionBox())) {
            (*it)->GetTarget()->TakeDamage(PROJECTILE_DAMAGE);
            delete *it;
            it = projectiles.erase(it);
        }
        else if ((*it)->ShouldRemove()) {
            delete *it;
            it = projectiles.erase(it);
        }
        else {
            ++it;
        }
    }

    // Collectible updates
    for (auto& c : collectibles) {
        if (c.active) {
            const int COLLECTIBLE_HITBOX_SIZE = 24;
            SDL_Rect centeredCollision = {
                c.rect.x + (c.rect.w - COLLECTIBLE_HITBOX_SIZE)/2,
                c.rect.y + (c.rect.h - COLLECTIBLE_HITBOX_SIZE)/2,
                COLLECTIBLE_HITBOX_SIZE,
                COLLECTIBLE_HITBOX_SIZE
            };

            if (CheckCollision(player->GetCollisionBox(), centeredCollision)) {
                c.active = false;
                collectiblesRemaining--;
                collectiblesCollected++;
                totalScore += 100;
            }
        }
    }

    // If gameover mid update
    if (!player->IsAlive()) {
        gameOver = true;
        currentState = GameState::GAME_OVER;
    }
}
void Game::ShootAllyProjectile(Enemy* target) {
    Vector2f startPos = ally->GetCenterPosition();
    projectiles.push_back(new Projectile(
        *texture_manager,
        target,
        startPos.x - 8,
        startPos.y - 8,
        600.0f,
        30,
        500.0f
    ));
}
Enemy* Game::FindNearestEnemyToAlly() {
    if (!ally) return nullptr;
    Vector2f allyPos = ally->GetCenterPosition();
    Enemy* nearest = nullptr;
    float minDist = FLT_MAX;

    for (auto enemy : enemies) {
        if (!enemy->IsAlive()) continue;
        Vector2f enemyPos = enemy->GetCenterPosition();
        float dist = Vector2f::Distance(allyPos, enemyPos);
        if (dist < minDist) {
            minDist = dist;
            nearest = enemy;
        }
    }
    return nearest;
}
// Collisoni
bool Game::CheckCollision(const SDL_Rect& a, const SDL_Rect& b) {
    bool xOverlap = (a.x <b.x + b.w) && (a.x + a.w > b.x);
    bool yOverlap = (a.y <b.y + b.h) && (a.y + a.h > b.y);

    return xOverlap && yOverlap;
}

void Game::Render() {
    SDL_RenderClear(renderer);


    if(currentState == GameState::MAIN_MENU) {
        SDL_RenderCopy(renderer, menuBackground, NULL, NULL);

        for(size_t i = 0; i < menuButtons.size(); i++) {
            std::string texName = menuButtons[i].id + "_btn";
            if(hoveredButton == static_cast<int>(i)) {
                texName += "_hover";
            }
            SDL_Texture* tex = texture_manager->GetTexture(texName);
            SDL_RenderCopy(renderer, tex, NULL, &menuButtons[i].rect);
        }
    }
    else if (currentState == GameState::REPLAY) {
        SDL_Rect cameraViewport = camera->GetViewport();
        tileMap->Render(renderer, cameraViewport);

        // Render collectibles
        for (const auto& c : collectibles) {
            if (c.active) {
                SDL_Rect destRect = {
                    c.rect.x - cameraViewport.x,
                    c.rect.y - cameraViewport.y,
                    64, 64
                };
                SDL_RenderCopy(renderer, collectibleTexture, &collectibleSrcRect, &destRect);
            }
        }

        // Render entities
        player->Render(renderer, cameraViewport);
        if (ally && ally->IsAlive()) ally->Render(renderer, cameraViewport);
        for (auto& enemy : enemies) enemy->Render(renderer, cameraViewport);
        for (auto& projectile : projectiles) projectile->Render(renderer, cameraViewport);
    }
    else if(currentState == GameState::HIGHSCORES_DISPLAY) {
            SDL_RenderCopy(renderer, menuBackground, NULL, NULL);

            // Title
            const int TITLE_Y = ScaleY(100);
            const int TITLE_FONT_SCALE = 3.0f;
            RenderText("TOP SCORES", windowWidth/2 - ScaleX(300), TITLE_Y, TITLE_FONT_SCALE);

            // Column
            const int HEADER_Y = TITLE_Y + ScaleY(80);  // Space after title
            const int ROW_START_Y = HEADER_Y + ScaleY(80); // Space after headers
            const int COLUMN_SPACING = ScaleX(200);

            // Colum size
            const int POS_WIDTH = ScaleX(100);
            const int NAME_WIDTH = ScaleX(200);
            const int SCORE_WIDTH = ScaleX(200);

            const int POS_X = windowWidth/2 - (NAME_WIDTH/2 + COLUMN_SPACING);
            const int NAME_X = windowWidth/2 - NAME_WIDTH/2;
            const int SCORE_X = windowWidth/2 + (COLUMN_SPACING - SCORE_WIDTH/2);

            // Column headers
            const float HEADER_SCALE = 2.2f;
            RenderText("POS", POS_X + POS_WIDTH/2 - ScaleX(60), HEADER_Y, HEADER_SCALE);
            RenderText("NAME", NAME_X + NAME_WIDTH/2 - ScaleX(100), HEADER_Y, HEADER_SCALE);
            RenderText("SCORE", SCORE_X + SCORE_WIDTH/2 - ScaleX(60), HEADER_Y, HEADER_SCALE);

            // Scores list
            const float ROW_SCALE = 2.0f;
            const int ROW_SPACING = ScaleY(80);
            for(size_t i = 0; i < highScores.size(); ++i) {
                int yPos = ROW_START_Y + i * ROW_SPACING;

                // Position number
                RenderNumber(i+1, POS_X + POS_WIDTH - ScaleX(60), yPos, ROW_SCALE);

                // Name
                std::string displayName = highScores[i].name;
                if(displayName.length() > 15) {  // Long name fix
                    displayName = displayName.substr(0, 12) + "...";
                }
                RenderText(displayName, NAME_X, yPos, ROW_SCALE);

                // Score
                RenderNumber(highScores[i].score, SCORE_X, yPos, ROW_SCALE);
            }

            // Centered back
            for(const auto& btn : menuButtons) {
                SDL_Rect centeredBtn = btn.rect;
                centeredBtn.y = ROW_START_Y + static_cast<int>(highScores.size() * ROW_SPACING) + ScaleY(100);
                centeredBtn.x = windowWidth/2 - centeredBtn.w/2;
                std::string texName = btn.id + "_btn";
                if(hoveredButton == &btn - &menuButtons[0]) {
                    texName += "_hover";
                }
                SDL_Texture* tex = texture_manager->GetTexture(texName);
                if(tex) SDL_RenderCopy(renderer, tex, NULL, &centeredBtn);
            }
        }
    else if (currentState == GameState::HIGHSCORE_ENTRY) {
        SDL_RenderCopy(renderer, menuBackground, NULL, NULL);
        RenderText("ENTER YOUR NAME: " + currentNameInput, windowWidth/2 - 200, windowHeight/2, 1.5f);
    }
    else if (currentState == GameState::SETTINGS) {
        SDL_RenderCopy(renderer, menuBackground, NULL, NULL);

        for (size_t i = 0; i < menuButtons.size(); i++) {
            std::string texName;

            if (menuButtons[i].id != "back") {
                texName = "resolution_btn_" + menuButtons[i].id;
                if (hoveredButton == static_cast<int>(i)) {
                    texName = "resolution_btn_hover_" + menuButtons[i].id;
                }
            } else {
                // Back button
                texName = "back_btn";
                if (hoveredButton == static_cast<int>(i)) {
                    texName += "_hover";
                }
            }

            SDL_Texture* tex = texture_manager->GetTexture(texName);
            if (tex) {
                SDL_RenderCopy(renderer, tex, NULL, &menuButtons[i].rect);
            }
        }
    }
    else if (currentState == GameState::GAME_OVER) {
        // Game over
        SDL_Rect screenRect = {0, 0, windowWidth, windowHeight};
        SDL_RenderCopy(renderer, gameOverTexture, NULL, &screenRect);

        // Position
        const int LABEL_X = windowWidth/2 - 200;
        const int NUMBER_X = windowWidth/2 + 200;
        const int BASE_Y = ScaleY(300);
        const int Y_SPACING = 80;

        // Score
        RenderText("SCORE:", LABEL_X, BASE_Y, 1.5f);
        RenderNumber(totalScore, NUMBER_X, BASE_Y, 1.5f);

        // Enemies Killed
        RenderText("ENEMIES:", LABEL_X, BASE_Y + Y_SPACING, 1.5f);
        RenderNumber(enemiesKilledTotal, NUMBER_X, BASE_Y + Y_SPACING, 1.5f);

        // Collectibles
        RenderText("COLLECTIBLES:", LABEL_X, BASE_Y + Y_SPACING*2, 1.5f);
        RenderNumber(collectiblesCollected, NUMBER_X, BASE_Y + Y_SPACING*2, 1.5f);

        RenderText("PRESS R TO RESTART", windowWidth/2 - 300, BASE_Y + Y_SPACING*4, 1.5f);
        RenderText("PRESS H FOR HIGHSCORE", windowWidth/2 - 350, BASE_Y + Y_SPACING*5, 1.5f);
        RenderText("PRESS P TO REPLAY LAST GAME", windowWidth/2 - 400, BASE_Y + Y_SPACING*6, 1.5f);
    }
    else if (currentState == GameState::PLAYING) {
        // Render game
        SDL_Rect cameraViewport = camera->GetViewport();

        if(!gameOver && !isChoosingUpgrade) {
            tileMap->Render(renderer, cameraViewport);

            // Collectible
            for (const auto& c : collectibles) {
                if (c.active) {
                    SDL_Rect destRect = {
                        c.rect.x - cameraViewport.x,
                        c.rect.y - cameraViewport.y,
                        64,
                        64
                    };
                    SDL_RenderCopy(renderer, collectibleTexture, &collectibleSrcRect, &destRect);
                }
            }

            player->Render(renderer, cameraViewport);

            if (ally && ally->IsAlive()) {
                ally->Render(renderer, cameraViewport);
            }

            for(auto& enemy : enemies) {
                enemy->Render(renderer, cameraViewport);
            }

            // Render bullets
            for(auto& projectile : projectiles) {
                projectile->Render(renderer, cameraViewport);
            }

        }

        // UI Elements
        else if (isChoosingUpgrade) {
    // Upgrade menu bg
    SDL_Rect overlay = {0, 0, windowWidth, windowHeight};
    SDL_RenderCopy(renderer, upgradeMenuTexture, NULL, &overlay);

    // Scales
    const int OPTION_WIDTH = ScaleX(300);
    const int OPTION_HEIGHT = ScaleY(400);
    const int SPACING = ScaleX(80);
    const int BASE_Y = ScaleY(300);
    const int TOTAL_WIDTH = (OPTION_WIDTH * 3) + (SPACING * 2);
    const int START_X = (windowWidth - TOTAL_WIDTH) / 2;


    // Pop up effect
    const int HOVER_OFFSET = ScaleY(15);
    const int HOVER_SIZE_INCREASE = ScaleY(30);

    // Fire Rate
    SDL_Rect fireRect = {
        START_X + (selectedUpgrade == 0 ? -HOVER_OFFSET : 0),
        BASE_Y + (selectedUpgrade == 0 ? -HOVER_OFFSET : 0),
        OPTION_WIDTH + (selectedUpgrade == 0 ? HOVER_SIZE_INCREASE : 0),
        OPTION_HEIGHT + (selectedUpgrade == 0 ? HOVER_SIZE_INCREASE : 0)
    };

    // Damage
    SDL_Rect damageRect = {
        START_X + OPTION_WIDTH + SPACING + (selectedUpgrade == 1 ? -HOVER_OFFSET : 0),
        BASE_Y + (selectedUpgrade == 1 ? -HOVER_OFFSET : 0),
        OPTION_WIDTH + (selectedUpgrade == 1 ? HOVER_SIZE_INCREASE : 0),
        OPTION_HEIGHT + (selectedUpgrade == 1 ? HOVER_SIZE_INCREASE : 0)
    };

    // Range
    SDL_Rect rangeRect = {
        START_X + (OPTION_WIDTH + SPACING) * 2 + (selectedUpgrade == 2 ? -HOVER_OFFSET : 0),
        BASE_Y + (selectedUpgrade == 2 ? -HOVER_OFFSET : 0),
        OPTION_WIDTH + (selectedUpgrade == 2 ? HOVER_SIZE_INCREASE : 0),
        OPTION_HEIGHT + (selectedUpgrade == 2 ? HOVER_SIZE_INCREASE : 0)
    };

    // Render options
    SDL_RenderCopy(renderer, upgradeFireTexture, NULL, &fireRect);
    SDL_RenderCopy(renderer, upgradeDamageTexture, NULL, &damageRect);
    SDL_RenderCopy(renderer, upgradeRangeTexture, NULL, &rangeRect);

    // Puščice
    if(texture_manager->HasTexture("arrow")) {
        SDL_Texture* arrowTex = texture_manager->GetTexture("arrow");
        const int ARROW_SIZE = ScaleX(40);

        // Lev
        SDL_Rect leftArrow = {
            fireRect.x - ScaleX(60),
            fireRect.y + (fireRect.h / 2 - ARROW_SIZE / 2),
            ARROW_SIZE,
            ARROW_SIZE
        };

        // Desen
        SDL_Rect rightArrow = {
            rangeRect.x + rangeRect.w + ScaleX(20),
            rangeRect.y + (rangeRect.h / 2 - ARROW_SIZE / 2),
            ARROW_SIZE,
            ARROW_SIZE
        };

        SDL_RenderCopyEx(renderer, arrowTex, NULL, &leftArrow, 180, NULL, SDL_FLIP_NONE);
        SDL_RenderCopy(renderer, arrowTex, NULL, &rightArrow);
    }
}

        // Health bar
        int startX = ScaleX(50);
        int startY = ScaleY(50);
        int spacing = ScaleX(10);
        int destHeartSize = ScaleY(64);
        int currentHealth = player->GetHealth();
        int maxHearts = player->GetMaxHealth() / HEALTH_PER_HEART;

        for(int i = 0; i < maxHearts; ++i) {
            int phase = CalculateHeartPhase(i, currentHealth);
            SDL_Rect srcRect = {
                phase * (112 / HEART_PHASES),
                0,
                112 / HEART_PHASES,
                srcHeartSize
            };

            SDL_Rect destRect = {
                startX + (destHeartSize + spacing) * i,
                startY,
                destHeartSize,
                destHeartSize
            };

            SDL_RenderCopy(renderer, heartTexture, &srcRect, &destRect);
        }

        // Wave number display
        if(!gameOver && !betweenWaves) {
            RenderWaveNumber();
            RenderTimer();
        }

        // Debug:
        if(debugMode) {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

            // Player hitbox
            SDL_Rect playerBox = player->GetCollisionBox();
            playerBox.x -= cameraViewport.x;
            playerBox.y -= cameraViewport.y;
            SDL_RenderDrawRect(renderer, &playerBox);

            // Enemy hit box
            for(auto& enemy : enemies) {
                SDL_Rect enemyBox = enemy->GetCollisionBox();
                enemyBox.x -= cameraViewport.x;
                enemyBox.y -= cameraViewport.y;
                SDL_RenderDrawRect(renderer, &enemyBox);
            }
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            for (const auto& c : collectibles) {
                if (c.active) {
                    SDL_Rect debugRect = {
                        c.rect.x - cameraViewport.x,
                        c.rect.y - cameraViewport.y,
                        c.rect.w,
                        c.rect.h
                    };
                    SDL_RenderDrawRect(renderer, &debugRect);
                }
            }
        }
    }

    SDL_RenderPresent(renderer);
}
void Game::SpawnCollectibles() {
    if (!betweenWaves) {
        collectibles.clear();
        collectiblesRemaining = BASE_COLLECTIBLES + (currentWave * COLLECTIBLES_PER_WAVE);

        Vector2f playerCenter = player->GetCenterPosition();
        const int BASE_DISTANCE = 600 + (currentWave * 100);

        for (int i = 0; i < collectiblesRemaining; ++i) {
            float angle = static_cast<float>(i) * (360.0f/collectiblesRemaining);
            float distance = BASE_DISTANCE + (rand() % 200);

            Vector2f offset = {
                static_cast<float>(cos(angle * (M_PI/180.0f)) * distance),
                static_cast<float>(sin(angle * (M_PI/180.0f)) * distance)
            };

            SDL_Rect pos = {
                static_cast<int>(playerCenter.x + offset.x - 16),
                static_cast<int>(playerCenter.y + offset.y - 16),
                32, 32
            };

            pos.x = std::clamp(pos.x, 0, 20000 - 32);
            pos.y = std::clamp(pos.y, 0, 20000 - 32);

            collectibles.push_back({pos, true});
        }
    }
}
void Game::RenderTimer() {
    if (!numbersTexture) return;

    int timerSeconds = static_cast<int>(waveTimeRemaining);
    if (timerSeconds < 0) timerSeconds = 0;
    std::string timeStr = std::to_string(timerSeconds);

    int startX = windowWidth - ScaleX(300);
    int startY = ScaleY(100);
    int digitWidth = ScaleX(NUMBER_WIDTH * NUMBER_SCALE);
    int digitHeight = ScaleY(NUMBER_HEIGHT * NUMBER_SCALE);
    int spacing = ScaleX(2);

    for (size_t i = 0; i < timeStr.size(); ++i) {
        char c = timeStr[i];
        int digit = c - '0';
        SDL_Rect srcRect = { digit * NUMBER_WIDTH, 0, NUMBER_WIDTH, NUMBER_HEIGHT };
        SDL_Rect destRect = {
            (startX + (digitWidth + spacing) * static_cast<int>(i)),
            (startY),
            (digitWidth),
            (digitHeight)
        };
        SDL_RenderCopy(renderer, numbersTexture, &srcRect, &destRect);
    }
}

int Game::CalculateHeartPhase(int index, int currentHealth) const {
    int fullHearts = currentHealth / HEALTH_PER_HEART;
    int remainder = currentHealth % HEALTH_PER_HEART;

    if (index < fullHearts) {
        return 0; // Full heart
    }
    if (index == fullHearts && remainder > 0) {
        float ratio = static_cast<float>(remainder) / HEALTH_PER_HEART;
        int originalPhase = static_cast<int>(std::round(ratio * (HEART_PHASES - 1)));
        return HEART_PHASES - 1 - originalPhase;
    }
    return HEART_PHASES - 1; // 0 heart
}

int Game::ScaleX(int original) const {
    return static_cast<int>(original * (windowWidth / 1920.0f));
}

int Game::ScaleY(int original) const {
    return static_cast<int>(original * (windowHeight / 1080.0f));
}


bool Game::Running() {
    return isRunning;
}

void Game::Clean() {
    if(isRecording) {
        FinalizeRecording();
    }
    delete player;
    delete texture_manager;
    delete ally;
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    if(currentState == GameState::PLAYING) {
        SaveGame();
    }
    std::cout << "Game cleaned" << std::endl;
}

void Game::SpawnEnemy() {
    const int MIN_SPAWN_DISTANCE = 500;
    const int MAX_SPAWN_DISTANCE = 800;

    SDL_Rect playerPos = player->GetPosition();

    // Random distance
    float angle = static_cast<float>(rand() % 360) * (M_PI / 180.0f);
    float distance = MIN_SPAWN_DISTANCE + static_cast<float>(rand()) / RAND_MAX * (MAX_SPAWN_DISTANCE - MIN_SPAWN_DISTANCE);

    int spawnX = playerPos.x + static_cast<int>(cos(angle) * distance);
    int spawnY = playerPos.y + static_cast<int>(sin(angle) * distance);

    // Map bounds
    spawnX = std::clamp(spawnX, 0, 20000);
    spawnY = std::clamp(spawnY, 0, 20000);

    // Lvl of enemy
    int level = 1;
    if(currentWave >= 5) {
        level = 3;
    }
    else if(currentWave >= 3) {
        level = (rand() % 100 < 80) ? 2 : 3;
    }
    else if(currentWave >= 1) {
        level = (rand() % 100 < 80) ? 1 : 2;
    }

    Enemy* e = GetPooledEnemy();
    if (e) {
        // Update pool enemy
        e->Revive(spawnX, spawnY, level);
        // Animation
        e->ReloadTexture(*texture_manager, "enemy" + std::to_string(level));
    } else {
        // New enemy
        e = new Enemy(
            *texture_manager,
            "enemy" + std::to_string(level),
            spawnX, spawnY,
            64, 64,
            32, 32,
            1,
            200.0f,
            player,
            50
        );
        enemyPool.push_back(e);
    }

    enemies.push_back(e);
}
void Game::RestartGame() {
    // Reset player
    delete player;
    player = new GameObject(*texture_manager, "player",
                           PLAYER_SPAWN_X, PLAYER_SPAWN_Y,
                           playerWidth, playerHeight,
                           frameWidth, frameHeight,
                           totalFrames, animationSpeed);

    // Reset ally
    if (ally) {
        ally->SetPosition(
            PLAYER_SPAWN_X + 100,  // Offset on 100px
            PLAYER_SPAWN_Y
        );
    }
    if(isRecording) FinalizeRecording();
    StartRecording();

    totalScore = 0;
    enemiesKilledTotal = 0;
    collectiblesCollected = 0;
    enemiesKilledThisWave = 0;

    // Reset enemies
    enemies.clear();
    for (auto& e : enemyPool) {
        e->Revive(-1000, -1000, 1);
    }
    // Reset upgrades
    FIRE_RATE = 1.0f;
    PROJECTILE_DAMAGE = 50;
    PISTOL_RANGE = 700.0f;

    // Reset game state
    currentWave = 0;
    waveTimer = 0.0f;
    waveTimeRemaining = WAVE_TIME_LIMIT;
    betweenWaves = true;
    gameOver = false;
    collectibles.clear();
    collectiblesRemaining = 0;

    // Reset camera
    camera->Reset(PLAYER_SPAWN_X + playerWidth/2,
                PLAYER_SPAWN_Y + playerHeight/2);
}
void Game::StartNewWave(int wave) {
    if (wave == -1) {
        waveTimeRemaining = WAVE_TIME_LIMIT; // Wave gameover
        currentWave++;
    }
    else currentWave = wave;

    betweenWaves = false;
    waveTimeRemaining = WAVE_TIME_LIMIT;

    // Clear enemy on new round
    if (wave == -1) {
        for (auto& e : enemyPool) {
            e->Revive(-1000, -1000, 1);
        }
        enemies.clear();
    }

    // Spawn collectible on new round
    if (wave == -1) {
        SpawnCollectibles();
    }

}
void Game::RenderWaveNumber() {
    if(!waveTextTexture || !numbersTexture) return;

    if(currentWave != cachedWaveNumber) {
        SDL_DestroyTexture(cachedWaveTexture);
        cachedWaveTexture = CreateWaveNumberTexture();
        cachedWaveNumber = currentWave;
    }

    if(cachedWaveTexture) {
        int texWidth, texHeight;
        SDL_QueryTexture(cachedWaveTexture, NULL, NULL, &texWidth, &texHeight);

        SDL_Rect destRect = {
            windowWidth - texWidth - ScaleX(50),
            ScaleY(50),
            texWidth,
            texHeight
        };

        SDL_RenderCopy(renderer, cachedWaveTexture, NULL, &destRect);
    }
}
// Pool
void Game::InitializeEnemyPool(int initialSize) {

    for (int i = 0; i < initialSize; ++i) {
        Enemy* e = new Enemy(
            *texture_manager,
            "enemy1",
            0, 0,
            32, 32,
            24, 24,
            1,
            200.0f,
            player,
            50
        );
        e->Revive(-1000, -1000, 1);
        enemyPool.push_back(e);
    }
}
// Pool pull
Enemy* Game::GetPooledEnemy() {
    for (auto& e : enemyPool) {
        if (!e->IsAlive()) {
            e->Revive(0, 0, 1);
            e->SetTarget(player);
            return e;
        }
    }

    Enemy* newEnemy = new Enemy(
    *texture_manager,
    "enemy",
    0, 0,
    32, 32,
    32, 32,
    3,
    2.0f,
    player,
    50
);
    newEnemy->SetTarget(player);
    enemyPool.push_back(newEnemy);
    return newEnemy;
}
void Game::UpdateWave(float deltaTime) {
    if (betweenWaves) {
        waveTimer += deltaTime;
        if (waveTimer >= waveStartDelay) {
            totalScore+=1000;
            StartNewWave();
            waveTimer = 0.0f;
        }
    } else {
        waveTimeRemaining -= deltaTime;

        // Game over check
        if (waveTimeRemaining <= 0.0f) {
            gameOver = true;
            currentState=GameState::GAME_OVER;
            return;
        }

        // Win check
        if (collectiblesRemaining <= 0) {
            // Clear enemies
            for (auto& enemy : enemies) {
                enemy->Revive(-1000, -1000, 1); // Reset
            }
            enemies.clear();

            betweenWaves = true;
            isChoosingUpgrade = true;
            selectedUpgrade = 0;
        }
        else {
            // Spawn infinetly
            spawnTimer += deltaTime;
            if (spawnTimer >= SPAWN_INTERVAL) {
                SpawnEnemy();
                spawnTimer = 0.0f;
            }
        }
    }
}
void Game::UpdateEnemies(float deltaTime) {
    // Enemy check between wave
    if (!betweenWaves && enemiesRemaining > 0) {
        spawnTimer += deltaTime;
        if (spawnTimer >= SPAWN_INTERVAL) {
            SpawnEnemy();
            enemiesRemaining--;
            spawnTimer = 0.0f;
        }
    }

    // Enemy update + collisions
    for (auto it = enemies.begin(); it != enemies.end();) {
        Enemy* enemy = *it;
        enemy->Update(deltaTime);

        // Hit check
        if (CheckCollision(enemy->GetCollisionBox(), player->GetCollisionBox())) {
            // Damage player
            if (player->GetDamageCooldown() <= 0.0f) {
                int lvl = enemy->GetLevel();
                int damage;
                switch (lvl) {
                    case 1:
                        damage = 5;
                    break;
                    case 2:
                        damage = 10;
                    break;
                    case 3:
                        damage = 20;
                    break;
                }
                player->TakeDamage(damage);
                player->SetDamageCooldown(player->GetDamageCooldownTime());
            }

            // Dmg enemy
            enemy->TakeDamage(50);
        }

        // Pool enemy
        if (!enemy->IsAlive()) {
            it = enemies.erase(it);
            enemiesKilledTotal++;
            totalScore+=25;
            enemiesKilledThisWave++;
        } else {
            ++it;
        }
    }

    // Debug:
    if (debugMode) {
        SDL_Rect playerBox = player->GetCollisionBox();
        SDL_Log("Player collision box: %d,%d %dx%d",
               playerBox.x, playerBox.y, playerBox.w, playerBox.h);

        for (auto& enemy : enemies) {
            SDL_Rect enemyBox = enemy->GetCollisionBox();
            SDL_Log("Enemy collision box: %d,%d %dx%d",
                   enemyBox.x, enemyBox.y, enemyBox.w, enemyBox.h);
        }
    }
}
void Game::UpdatePlayer(float deltaTime) {
    if (player) {
        // Update location
        player->Update(deltaTime);

        // Get location
        SDL_Rect playerPos = player->GetPosition();

        // Center
        int playerCenterX = playerPos.x + playerPos.w / 2;
        int playerCenterY = playerPos.y + playerPos.h / 2;

        // Camera update
        camera->Update(playerCenterX, playerCenterY);

        // Map update
        tileMap->Update(deltaTime);

        // Projectiles
        fireTimer += deltaTime;
        if (fireTimer >= 1.0f/FIRE_RATE) {
            Enemy* nearest = FindNearestEnemy();
            if (nearest && DistanceToPlayer(nearest) <= PISTOL_RANGE) {
                ShootProjectile(nearest);
            }
            fireTimer = 0.0f;
        }
    }
}
Enemy* Game::FindNearestEnemy() {
    Enemy* nearest = nullptr;
    float minDistance = FLT_MAX;
    Vector2f playerPos = player->GetCenterPosition();

    //check for all
    for (auto& enemy : enemies) {
        Vector2f enemyPos = enemy->GetCenterPosition();
        float distance = sqrt(pow(enemyPos.x - playerPos.x, 2) +
                            pow(enemyPos.y - playerPos.y, 2));
        if (distance < minDistance) {
            minDistance = distance;
            nearest = enemy;


        }
    }


    return nearest;
}
void Game::ShootProjectile(Enemy* target) {
    projectiles.push_back(new Projectile(
        *texture_manager,
        target,
        player->GetPosition().x,
        player->GetPosition().y,
        PROJECTILE_SPEED,
        PROJECTILE_DAMAGE,
        PISTOL_RANGE
    ));
    fireTimer = 0.0f;
}


// Movment
void Game::ProcessInput() {
    const Uint8* keystate = SDL_GetKeyboardState(NULL);
    float hitrost = 400.0f;// Speed
    float vx = 0, vy = 0;
    if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LSHIFT]) {
        hitrost *= 1.5f; // Run
    }

    if (keystate[SDL_SCANCODE_W]) vy = -hitrost;
    if (keystate[SDL_SCANCODE_S]) vy = hitrost;
    if (keystate[SDL_SCANCODE_A]) vx = -hitrost;
    if (keystate[SDL_SCANCODE_D]) vx = hitrost;

    if (vx != 0 || vy != 0) {
        float dolzina = sqrt(vx * vx + vy * vy);
        vx = (vx / dolzina) * hitrost;
        vy = (vy / dolzina) * hitrost;

        if (abs(vx) > abs(vy)) {
            player->SetDirection(vx > 0 ? GameObject::Direction::RIGHT : GameObject::Direction::LEFT);
        } else {
            player->SetDirection(vy > 0 ? GameObject::Direction::DOWN : GameObject::Direction::UP);
        }
    }

    player->SetVelocity(vx, vy);
}
SDL_Texture* Game::CreateWaveNumberTexture() {
    // Load scale image
    SDL_Surface* waveSurface = IMG_Load("assets/textures/ui/wave.png");
    SDL_Surface* numbersSurface = IMG_Load("assets/textures/ui/numbers.png");

    if(!waveSurface || !numbersSurface) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load wave/number textures");
        return nullptr;
    }

    // Scale surface
    SDL_Surface* scaledWave = SDL_CreateRGBSurfaceWithFormat(0,
        waveSurface->w * WAVE_SCALE,
        waveSurface->h * WAVE_SCALE,
        32,
        SDL_PIXELFORMAT_RGBA32);

    SDL_BlitScaled(waveSurface, NULL, scaledWave, NULL);

    // Scale
    int numberWidth = NUMBER_WIDTH * NUMBER_SCALE;
    int numberHeight = NUMBER_HEIGHT * NUMBER_SCALE;

    SDL_Surface* combinedSurface = SDL_CreateRGBSurfaceWithFormat(0,
        scaledWave->w + (std::to_string(currentWave).length() * (numberWidth + 2)),
        std::max(scaledWave->h, numberHeight),
        32,
        SDL_PIXELFORMAT_RGBA32);

    // Scale wave
    SDL_Rect waveDest = {0, (combinedSurface->h - scaledWave->h)/2, scaledWave->w, scaledWave->h};
    SDL_BlitSurface(scaledWave, NULL, combinedSurface, &waveDest);

    // Scale number
    int xOffset = waveDest.w;
    std::string waveStr = std::to_string(currentWave);

    for(char c : waveStr) {
        int digit = c - '0';
        SDL_Rect srcRect = {digit * NUMBER_WIDTH, 0, NUMBER_WIDTH, NUMBER_HEIGHT};
        SDL_Rect destRect = {
            xOffset,
            (combinedSurface->h - numberHeight)/2,
            numberWidth,
            numberHeight
        };

        SDL_BlitScaled(numbersSurface, &srcRect, combinedSurface, &destRect);
        xOffset += numberWidth + 2;
    }

    // Final textura
    SDL_Texture* result = SDL_CreateTextureFromSurface(renderer, combinedSurface);

    // Clean
    SDL_FreeSurface(waveSurface);
    SDL_FreeSurface(numbersSurface);
    SDL_FreeSurface(scaledWave);
    SDL_FreeSurface(combinedSurface);

    return result;
}
float Game::DistanceToPlayer(Enemy* enemy) {
    if (!player || !enemy) return FLT_MAX;

    Vector2f playerPos = player->GetCenterPosition();
    Vector2f enemyPos = enemy->GetCenterPosition();

    float dx = playerPos.x - enemyPos.x;
    float dy = playerPos.y - enemyPos.y;

    return sqrt(dx * dx + dy * dy);
}

void Game::ApplyUpgrade(int choice) {
    switch(choice) {
        case 0: // Fire Rate
            FIRE_RATE *= 1.2f;
        break;
        case 1: // Damage
            PROJECTILE_DAMAGE +=100;
        break;
        case 2: // Range
            PISTOL_RANGE +=1.2f;
        break;
    }
}
void Game::CreateMenuLayout() {
    menuButtons.clear();

    if(currentState == GameState::MAIN_MENU) {
        const int BUTTON_WIDTH = ScaleX(400);
        const int BUTTON_HEIGHT = ScaleY(100);
        const int SPACING = ScaleY(50);
        const int START_Y = ScaleY(300);

        menuButtons.push_back({"play", {windowWidth/2 - BUTTON_WIDTH/2, START_Y, BUTTON_WIDTH, BUTTON_HEIGHT}});
        menuButtons.push_back({"load", {windowWidth/2 - BUTTON_WIDTH/2, START_Y + 1*(BUTTON_HEIGHT + SPACING), BUTTON_WIDTH, BUTTON_HEIGHT}});
        menuButtons.push_back({"settings", {windowWidth/2 - BUTTON_WIDTH/2, START_Y + 2*(BUTTON_HEIGHT + SPACING), BUTTON_WIDTH, BUTTON_HEIGHT}});
        menuButtons.push_back({"scores", {windowWidth/2 - BUTTON_WIDTH/2, START_Y + 3*(BUTTON_HEIGHT + SPACING), BUTTON_WIDTH, BUTTON_HEIGHT}});
        menuButtons.push_back({"quit", {windowWidth/2 - BUTTON_WIDTH/2, START_Y + 4*(BUTTON_HEIGHT + SPACING), BUTTON_WIDTH, BUTTON_HEIGHT}});
    }
    else if(currentState == GameState::HIGHSCORES_DISPLAY) {
        SDL_Texture* backTex = texture_manager->GetTexture("back_btn");
        int texW, texH;
        SDL_QueryTexture(backTex, NULL, NULL, &texW, &texH);

        menuButtons.push_back({
            "back",
            {windowWidth/2 - texW/2, windowHeight - ScaleY(200), texW, texH}
        });
    }
    else if(currentState == GameState::SETTINGS) {
        const int BUTTON_SPACING = ScaleY(30);
        int yPos = ScaleY(300);

        for(const auto& res : resolutions) {
            std::string resStr = std::to_string(res.first) + "x" + std::to_string(res.second);
            std::string texName = "resolution_btn_" + resStr;
            SDL_Texture* tex = texture_manager->GetTexture(texName);
            if(tex) {
                int texWidth, texHeight;
                SDL_QueryTexture(tex, NULL, NULL, &texWidth, &texHeight);
                menuButtons.push_back({
                    resStr,
                    {windowWidth/2 - texWidth/2, yPos, texWidth, texHeight}
                });
                yPos += texHeight + BUTTON_SPACING;
            }
        }

        // Back button
        SDL_Texture* backTex = texture_manager->GetTexture("back_btn");
        int backWidth, backHeight;
        SDL_QueryTexture(backTex, NULL, NULL, &backWidth, &backHeight);
        menuButtons.push_back({
            "back",
            {windowWidth/2 - backWidth/2, yPos, backWidth, backHeight}
        });
    }
}

void Game::UpdateButtonHover(int mouseX, int mouseY) {
    hoveredButton = -1;
    int winW, winH;
    SDL_GetWindowSize(window, &winW, &winH);
    float scaleX = static_cast<float>(windowWidth) / winW;
    float scaleY = static_cast<float>(windowHeight) / winH;

    SDL_Point mousePos = {
        static_cast<int>(mouseX * scaleX),
        static_cast<int>(mouseY * scaleY)
    };

    for (size_t i = 0; i < menuButtons.size(); i++) {
        if (SDL_PointInRect(&mousePos, &menuButtons[i].rect)) {
            hoveredButton = static_cast<int>(i);
            break;
        }
    }
}

void Game::HandleMenuClick(int mouseX, int mouseY) {
    // Scale mouse location
    int winW, winH;
    SDL_GetWindowSize(window, &winW, &winH);
    float scaleX = static_cast<float>(windowWidth) / winW;
    float scaleY = static_cast<float>(windowHeight) / winH;

    SDL_Point mousePos = {
        static_cast<int>(mouseX * scaleX),
        static_cast<int>(mouseY * scaleY)
    };

    // Check all menu buttons
    for(size_t i = 0; i < menuButtons.size(); i++) {
        if(SDL_PointInRect(&mousePos, &menuButtons[i].rect)) {
            if(currentState == GameState::MAIN_MENU) {
                if(menuButtons[i].id == "play") {
                    RestartGame();
                    currentState = GameState::PLAYING;
                    StartNewWave();
                }
                else if(menuButtons[i].id == "load") {
                    if(LoadGame()) {
                        currentState = GameState::PLAYING;
                        player->SetVelocity(0, 0);
                        player->SetDirection(GameObject::Direction::DOWN);
                    }
                }
                else if(menuButtons[i].id == "settings") {
                    currentState = GameState::SETTINGS;
                    CreateMenuLayout();
                }
                else if(menuButtons[i].id == "scores") {
                    currentState = GameState::HIGHSCORES_DISPLAY;
                    CreateMenuLayout();
                }
                else if(menuButtons[i].id == "quit") {
                    isRunning = false;
                }
                return;
            }
            else if(currentState == GameState::SETTINGS) {
                if(menuButtons[i].id.find('x') != std::string::npos) {
                    // Handle resolution change
                    size_t separator = menuButtons[i].id.find('x');
                    int width = std::stoi(menuButtons[i].id.substr(0, separator));
                    int height = std::stoi(menuButtons[i].id.substr(separator+1));
                    ChangeResolution(width, height);
                }
                else if(menuButtons[i].id == "back") {
                    currentState = GameState::MAIN_MENU;
                    CreateMenuLayout();
                }
                return;
            }

            else if(currentState == GameState::HIGHSCORES_DISPLAY) {
                if(menuButtons[i].id == "back") {
                    currentState = GameState::MAIN_MENU;
                    CreateMenuLayout();
                    return;
                }
            }
        }
        if(menuButtons[i].id == "back") {
            SDL_Log("BACK BUTTON CLICKED");
            currentState = GameState::MAIN_MENU;
            CreateMenuLayout();
            return;
        }

    }
}



void Game::ChangeResolution(int width, int height) {
    // Destroy old texture
    SDL_DestroyRenderer(renderer);
    texture_manager->ClearTextures();

    // Get window size
    windowWidth = width;
    windowHeight = height;
    SDL_SetWindowSize(window, width, height);
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

    // New render
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    texture_manager->UpdateRenderer(renderer);

    // Reload all texture
    ReloadAllTextures();


    // Reset
    SDL_DestroyTexture(cachedWaveTexture);
    cachedWaveTexture = nullptr;
    cachedWaveNumber = -1;

    // Force UI elements
    CreateMenuLayout();

    // Reset camera + tilemap
    delete camera;
    camera = new Camera(windowWidth, windowHeight);
    camera->SetMapBounds(20000, 20000);

    delete tileMap;
    tileMap = new TileMap(*texture_manager, *camera);

    // Reset on player
    Vector2f playerPos = player->GetCenterPosition();
    camera->Reset(playerPos.x, playerPos.y);

    // New collectibles
    if (!betweenWaves) {
        SpawnCollectibles();
    }

    // Force
    SDL_RenderClear(renderer);
    Render();
    SDL_RenderPresent(renderer);
}

void Game::RenderNumber(int number, int x, int y, float scale) {
    if (!numbersTexture) return;

    float renderScale = scale * (static_cast<float>(windowWidth) / 1920.0f);
    const int DIGIT_WIDTH = 16 * renderScale;
    const int SPACING = 2 * renderScale;

    // Convert coordinates
    int renderX = ScaleX(x);
    int renderY = ScaleY(y);

    std::string numStr = std::to_string(number);

    for (size_t i = 0; i < numStr.size(); ++i) {
        char c = numStr[i];
        if (c < '0' || c > '9') continue;

        int digit = c - '0';
        SDL_Rect srcRect = { digit * 16, 0, 16, 16 };
        SDL_Rect destRect = {
            renderX + static_cast<int>(i * (DIGIT_WIDTH + SPACING)),
            renderY,
            static_cast<int>(DIGIT_WIDTH),
            static_cast<int>(16 * renderScale)
        };
        SDL_RenderCopy(renderer, numbersTexture, &srcRect, &destRect);
    }
}
void Game::SaveGame() {
    CreateGameDirectories();
    std::string savePath = GetSavePath("save.dat");

    SDL_Log("Saving game to: %s", savePath.c_str());

    SDL_Log("Saving to: %s", savePath.c_str());
    std::ofstream saveFile(savePath, std::ios::binary);
    if (!saveFile) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to save game!");
        return;
    }
    if (!saveFile) {
        SDL_Log("Failed to save game!");
        return;
    }

    // Save player
    Vector2f playerPos = player->GetCenterPosition();
    saveFile.write(reinterpret_cast<const char*>(&playerPos.x), sizeof(float));
    saveFile.write(reinterpret_cast<const char*>(&playerPos.y), sizeof(float));
    int health = player->GetHealth();
    saveFile.write(reinterpret_cast<const char*>(&health), sizeof(int));

    saveFile.write(reinterpret_cast<const char*>(&totalScore), sizeof(int));

    // Savewave
    saveFile.write(reinterpret_cast<const char*>(&currentWave), sizeof(int));

    // Save collectibles
    int collectibleCount = collectibles.size();
    saveFile.write(reinterpret_cast<const char*>(&collectibleCount), sizeof(int));
    for (const auto& c : collectibles) {
        saveFile.write(reinterpret_cast<const char*>(&c.rect.x), sizeof(int));
        saveFile.write(reinterpret_cast<const char*>(&c.rect.y), sizeof(int));
        saveFile.write(reinterpret_cast<const char*>(&c.active), sizeof(bool));
    }

    // Save upgrades
    saveFile.write(reinterpret_cast<const char*>(&PISTOL_RANGE), sizeof(float));
    saveFile.write(reinterpret_cast<const char*>(&FIRE_RATE), sizeof(float));
    saveFile.write(reinterpret_cast<const char*>(&PROJECTILE_DAMAGE), sizeof(int));

    // Save wave state
    saveFile.write(reinterpret_cast<const char*>(&betweenWaves), sizeof(bool));

    // Save the timer
    saveFile.write(reinterpret_cast<const char*>(&waveTimeRemaining), sizeof(float));


    saveFile.close();
    if (saveFile.fail()) {
        SDL_Log("Error occurred during saving!");
    }
    SDL_Log("Game saved successfully to: %s", savePath.c_str());
}

bool Game::LoadGame() {
    for (auto& e : enemies) e->Revive(-1000, -1000, 1);
    enemies.clear();
    for (auto& p : projectiles) delete p;
    projectiles.clear();
    CreateGameDirectories();
    std::string savePath = GetSavePath("save.dat");

    std::ifstream loadFile(savePath, std::ios::binary);
    if (!loadFile) {
        SDL_Log("No save file found at: %s", savePath.c_str());
        return false;
    }

    // Load player
    Vector2f playerPos;
    loadFile.read(reinterpret_cast<char*>(&playerPos.x), sizeof(float));
    loadFile.read(reinterpret_cast<char*>(&playerPos.y), sizeof(float));


    // Set player, camera
    Vector2f centerPos = player->GetCenterPosition();
    camera->Reset(centerPos.x, centerPos.y);
    camera->SetTarget(player);

    if (ally) {
        ally->SetPosition(centerPos.x + 100, centerPos.y);
        ally->Revive(centerPos.x + 100, centerPos.y, 100);
    }

    // Load health
    int health;
    loadFile.read(reinterpret_cast<char*>(&health), sizeof(int));

    loadFile.read(reinterpret_cast<char*>(&totalScore), sizeof(int));

    player->SetHealth(std::max(0, health)); // Brez gameover savea
    gameOver = (health <= 0); // Game over check

    // Load wave
    loadFile.read(reinterpret_cast<char*>(&currentWave), sizeof(int));
    betweenWaves = false;
    waveTimeRemaining = WAVE_TIME_LIMIT;

    // Load collectibles
    int collectibleCount;
    loadFile.read(reinterpret_cast<char*>(&collectibleCount), sizeof(int));
    collectibles.clear();
    for (int i = 0; i < collectibleCount; ++i) {
        Collectible c;
        loadFile.read(reinterpret_cast<char*>(&c.rect.x), sizeof(int));
        loadFile.read(reinterpret_cast<char*>(&c.rect.y), sizeof(int));
        loadFile.read(reinterpret_cast<char*>(&c.active), sizeof(bool));

        // Chat said i need to do this he has no clue what he is doing
        c.rect.w = 32;
        c.rect.h = 32;

        // Validate position
        c.rect.x = std::clamp(c.rect.x, 0, 20000 - c.rect.w);
        c.rect.y = std::clamp(c.rect.y, 0, 20000 - c.rect.h);

        collectibles.push_back(c);
    }
    collectiblesRemaining = std::count_if(
    collectibles.begin(), collectibles.end(),
    [](const Collectible& c) { return c.active; }
);

    // Force spawn
    if (collectibles.empty() && !betweenWaves) {
        SpawnCollectibles();
    }

    // Load upgrades
    loadFile.read(reinterpret_cast<char*>(&PISTOL_RANGE), sizeof(float));
    loadFile.read(reinterpret_cast<char*>(&FIRE_RATE), sizeof(float));
    loadFile.read(reinterpret_cast<char*>(&PROJECTILE_DAMAGE), sizeof(int));

    // Reset combat state
    enemies.clear();
    for (auto& p : projectiles) delete p;
    projectiles.clear();
    spawnTimer = 0.0f;
    fireTimer = 0.0f;

    // Reset enemy pool
    for (auto& enemy : enemyPool) {
        enemy->Revive(-1000, -1000, 1);
    }

    // Initialize wave system
    StartNewWave(currentWave);
    betweenWaves = (collectiblesRemaining <= 0);
    isChoosingUpgrade = false;

    // Refresh UI
    cachedWaveNumber = -1;
    SDL_DestroyTexture(cachedWaveTexture);
    cachedWaveTexture = nullptr;

    // Load wave state
    loadFile.read(reinterpret_cast<char*>(&betweenWaves), sizeof(bool));

    // Load the timer
    loadFile.read(reinterpret_cast<char*>(&waveTimeRemaining), sizeof(float));


    // Force camera update
    Vector2f playerCenter = player->GetCenterPosition();
    camera->Update(playerCenter.x, playerCenter.y);


    loadFile.close();


    SDL_Log("Game loaded successfully. Player at: %d,%d",
          player->GetPosition().x, player->GetPosition().y);
    return true;
}
void Game::SaveHighScores() {
    CreateGameDirectories();
    std::string path = GetSavePath("highscores.dat");

    std::ofstream file(path, std::ios::binary);
    if (file.is_open()) {
        size_t count = highScores.size();
        file.write(reinterpret_cast<char*>(&count), sizeof(size_t));
        for (const auto& hs : highScores) {
            size_t nameLen = hs.name.size();
            file.write(reinterpret_cast<char*>(&nameLen), sizeof(size_t));
            file.write(hs.name.c_str(), nameLen);
            file.write(reinterpret_cast<const char*>(&hs.score), sizeof(int));
        }
    }
}

void Game::LoadHighScores() {
    CreateGameDirectories();
    std::string path = GetSavePath("highscores.dat");

    std::ifstream file(path, std::ios::binary);
    highScores.clear();
    if (file.is_open()) {
        size_t count;
        file.read(reinterpret_cast<char*>(&count), sizeof(size_t));
        for (size_t i = 0; i < count; ++i) {
            HighScore hs;
            size_t nameLen;
            file.read(reinterpret_cast<char*>(&nameLen), sizeof(size_t));
            hs.name.resize(nameLen);
            file.read(&hs.name[0], nameLen);
            file.read(reinterpret_cast<char*>(&hs.score), sizeof(int));
            highScores.push_back(hs);
        }
    }
}
void Game::HandleHighscoreInput(const SDL_Event& event) {
    SDL_Keycode key = event.key.keysym.sym;
    if (key == SDLK_RETURN && !currentNameInput.empty()) {
        // Add score and sort
        highScores.push_back({currentNameInput, totalScore});
        std::sort(highScores.begin(), highScores.end(),
            [](const HighScore& a, const HighScore& b) { return a.score > b.score; });
        if (highScores.size() > 5) highScores.resize(5);
        SaveHighScores();
        currentState = GameState::MAIN_MENU;
    } else if (key == SDLK_BACKSPACE && !currentNameInput.empty()) {
        currentNameInput.pop_back();
    } else if (currentNameInput.length() < MAX_NAME_LENGTH) {
        if (key >= SDLK_a && key <= SDLK_z) {
            currentNameInput += toupper(static_cast<char>(key));
        }
    }
}
void Game::RenderText(const std::string& text, int x, int y, float scale) {
    if (!lettersTexture) return;

    int renderX = ScaleX(x);
    int renderY = ScaleY(y);
    float renderScale = scale * (static_cast<float>(windowWidth) / 1920.0f);

    const int destWidth = LETTER_WIDTH * renderScale;
    const int destHeight = LETTER_HEIGHT * renderScale;
    for (size_t i = 0; i < text.size(); ++i) {
        char c = text[i];
        int index = toupper(c) - 'A';
        if (c == ' ') continue;
        if (index < 0 || index >= 26) index = 26;

        SDL_Rect src = { index * LETTER_WIDTH, 0, LETTER_WIDTH, LETTER_HEIGHT };
        SDL_Rect dest = {
            renderX + static_cast<int>(i * destWidth * 1.2f),
            renderY,
            destWidth,
            destHeight
        };
        SDL_RenderCopy(renderer, lettersTexture, &src, &dest);
    }
}
void Game::StartRecording() {
    currentReplayFrame = 0;
    totalRecordedTime = 0.0f;
    currentReplayFile = GetSavePath("last_replay.bin");
    std::ofstream file(currentReplayFile, std::ios::binary);

    ReplayHeader header;
    strncpy(header.magic, "RPLY", 4);
    header.frameCount = 0;
    header.totalTime = 0.0f;
    header.finalScore = 0;
    header.finalWave = 0;

    file.write(reinterpret_cast<char*>(&header), sizeof(ReplayHeader));
    file.close();
    isRecording = true;
}

void Game::RecordFrame(float deltaTime) {
    SDL_Log("Recording frame %d", currentReplayFrame);
    std::ofstream file(currentReplayFile, std::ios::binary | std::ios::app);

    ReplayFrame frame;
    frame.timestamp = totalRecordedTime;
    totalRecordedTime += deltaTime;
    frame.playerPos = player->GetCenterPosition();
    frame.playerVelocity = player->GetVelocity();
    frame.playerFrameIndex = player->GetCurrentFrameIndex();


    if(ally) {
        frame.allyPos = ally->GetCenterPosition();
        frame.allyVelocity = ally->GetVelocity();
        frame.allyFrameIndex = ally->GetCurrentFrameIndex();
        frame.allyActive = ally->IsAlive();
    }

    // Record enemies
    frame.enemyCount = 0;
    std::vector<std::pair<Vector2f, int>> liveEnemies;
    for (auto& e : enemies) {
        if (e->IsAlive()) {
            SDL_Rect enemyPos = e->GetPosition();
            liveEnemies.emplace_back(
                Vector2f(enemyPos.x, enemyPos.y),
                e->GetLevel()
            );
        }
    }
    frame.enemyCount = liveEnemies.size();

    // Record projectiles
    frame.projectileCount = projectiles.size();

    // Record collectibles
    frame.collectibleCount = 0;
    std::vector<Vector2f> activeCollectibles;
    for (auto& c : collectibles) {
        if (c.active) {
            activeCollectibles.emplace_back(c.rect.x, c.rect.y);
        }
    }
    frame.collectibleCount = activeCollectibles.size();

    // Write frame header
    file.write(reinterpret_cast<char*>(&frame), sizeof(ReplayFrame));

    // Write enemy data
    for(auto& [pos, lvl] : liveEnemies) {
        file.write(reinterpret_cast<char*>(&pos), sizeof(Vector2f));
        file.write(reinterpret_cast<char*>(&lvl), sizeof(int));
    }

    // Write projectile positions
    for (auto& p : projectiles) {
        SDL_Rect projectileRect = p->GetPosition();
        Vector2f pos(projectileRect.x, projectileRect.y);
        file.write(reinterpret_cast<char*>(&pos), sizeof(Vector2f));
    }

    // Write collectibles
    for(auto& pos : activeCollectibles) {
        file.write(reinterpret_cast<char*>(&pos), sizeof(Vector2f));
    }
    currentReplayFrame++;

    file.close();
}

void Game::FinalizeRecording() {
    if (!isRecording) return;
    isRecording = false;

    std::fstream file(currentReplayFile, std::ios::in | std::ios::out | std::ios::binary);
    ReplayHeader header;
    file.read(reinterpret_cast<char*>(&header), sizeof(ReplayHeader));

    header.totalTime = totalRecordedTime;
    header.finalScore = totalScore;
    header.finalWave = currentWave;
    header.frameCount = currentReplayFrame;

    file.seekp(0);
    file.write(reinterpret_cast<char*>(&header), sizeof(ReplayHeader));
    file.close();
}
void Game::StartReplay(const std::string& filename) {
    replayFile.open(filename, std::ios::binary);
    if (!replayFile) {
        SDL_Log("Failed to open replay file: %s", filename.c_str());
        return;
    }

    ReplayHeader header;
    replayFile.read(reinterpret_cast<char*>(&header), sizeof(ReplayHeader));

    if (memcmp(header.magic, "RPLY", 4) != 0) {
        SDL_Log("Invalid replay file format");
        replayFile.close();
        return;
    }


    ReplayFrame firstFrame;
    replayFile.read(reinterpret_cast<char*>(&firstFrame), sizeof(ReplayFrame));
    camera->Reset(firstFrame.playerPos.x, firstFrame.playerPos.y);
    replayFile.seekg(sizeof(ReplayHeader));

    currentState = GameState::REPLAY;
    isReplaying = true;
    replayTimer = 0.0f;
}

void Game::UpdateReplay(float deltaTime) {
    if (!isReplaying) return;

    replayTimer += deltaTime * replaySpeed;

    while (true) {
        tileMap->Update(deltaTime);
        ReplayFrame frame;
        if (!replayFile.read(reinterpret_cast<char*>(&frame), sizeof(ReplayFrame))) {
            if (replayFile.eof()) {
                isReplaying = false;
                currentState = GameState::GAME_OVER;
                gameOver = true;
                replayFile.close();
            }
            break;
        }

        if (frame.timestamp > replayTimer) {
            replayFile.seekg(-static_cast<int>(sizeof(ReplayFrame)), std::ios::cur);
            break;
        }

        // Update player and camera
        player->SetPosition(frame.playerPos.x, frame.playerPos.y);

        camera->Reset(frame.playerPos.x, frame.playerPos.y);

        player->SetVelocity(frame.playerVelocity.x, frame.playerVelocity.y);
        player->SetCurrentFrameIndex(frame.playerFrameIndex);
        player->Update(deltaTime);

        ally->Revive(frame.playerPos.x + 100, frame.playerPos.y,100);

    if(ally) {
        ally->SetPosition(frame.allyPos.x, frame.allyPos.y);
        ally->SetVelocity(frame.allyVelocity.x, frame.allyVelocity.y);
        ally->SetCurrentFrameIndex(frame.allyFrameIndex);
        ally->Update(deltaTime);
    }

        // Clear old entities
        enemies.clear();
        for (auto& p : projectiles) delete p;
        projectiles.clear();
        collectibles.clear();

        // Restore enemies
        for (uint16_t i = 0; i < frame.enemyCount; i++) {
            Vector2f pos;
            int level;
            if (!replayFile.read(reinterpret_cast<char*>(&pos), sizeof(Vector2f)) ||
                !replayFile.read(reinterpret_cast<char*>(&level), sizeof(int))) {
                SDL_Log("Replay file corrupted!");
                isReplaying = false;
                replayFile.close();
                break;
            }
            Enemy* e = GetPooledEnemy();
            if (e) {
                e->Revive(pos.x, pos.y, level);
                enemies.push_back(e);
            }
        }
        if (!isReplaying) break;

        // Restore projectiles
        for (uint16_t i = 0; i < frame.projectileCount; i++) {
            Vector2f pos;
            if (!replayFile.read(reinterpret_cast<char*>(&pos), sizeof(Vector2f))) {
                SDL_Log("Replay file corrupted!");
                isReplaying = false;
                replayFile.close();
                break;
            }
            Enemy* nearest = FindNearestEnemy();
            if (nearest) {
                projectiles.push_back(new Projectile(
                    *texture_manager,
                    nearest,
                    pos.x,
                    pos.y,
                    600.0f,
                    30,
                    500.0f
                ));
            }
        }
        if (!isReplaying) break;

        // Restore collectibles
        for (uint16_t i = 0; i < frame.collectibleCount; i++) {
            Vector2f pos;
            if (!replayFile.read(reinterpret_cast<char*>(&pos), sizeof(Vector2f))) {
                SDL_Log("Replay file corrupted!");
                isReplaying = false;
                replayFile.close();
                break;
            }
            collectibles.push_back({{static_cast<int>(pos.x), static_cast<int>(pos.y), 32, 32}, true});
        }
        if (!isReplaying) {
            break;
        }
    }
}

void Game::ReloadAllTextures() {

    texture_manager->ClearTextures();


    texture_manager->LoadTexture("assets/textures/menu/background.png", "menu_bg");
    texture_manager->LoadTexture("assets/textures/menu/button.png", "button");
    texture_manager->LoadTexture("assets/textures/menu/button_hover.png", "button_hover");
    texture_manager->LoadTexture("assets/textures/menu/load_btn.png", "load_btn");
    texture_manager->LoadTexture("assets/textures/menu/load_btn_hover.png", "load_btn_hover");

    texture_manager->LoadTexture("assets/textures/ui/letters.png", "letters");


    texture_manager->LoadTexture("assets/textures/Player/Turtle-Wick.png", "player");
    texture_manager->LoadTexture("assets/textures/Player/ally.png", "ally");


    texture_manager->LoadTexture("assets/textures/Tiles/water_sheet.png", "water");

    texture_manager->LoadTexture("assets/textures/Enemys/test.png", "enemy1");
    texture_manager->LoadTexture("assets/textures/Enemys/newenemy2.png", "enemy2");
    texture_manager->LoadTexture("assets/textures/Enemys/newenemy3.png", "enemy3");


    texture_manager->LoadTexture("assets/textures/ui/GameOver.png", "game_over");
    texture_manager->LoadTexture("assets/textures/ui/Heart.png", "heart");
    texture_manager->LoadTexture("assets/textures/Weapons/bullet.png", "bullet");
    texture_manager->LoadTexture("assets/textures/ui/numbers.png", "numbers");
    texture_manager->LoadTexture("assets/textures/ui/wave.png", "wave_text");
    texture_manager->LoadTexture("assets/textures/collectible2.png", "collectible");

    texture_manager->LoadTexture("assets/textures/ui/upgrade_menu.png", "upgrade_menu");
    texture_manager->LoadTexture("assets/textures/ui/upgrade_fire.png", "upgrade_fire_rate");
    texture_manager->LoadTexture("assets/textures/ui/upgrade_damage.png", "upgrade_damage");
    texture_manager->LoadTexture("assets/textures/ui/upgrade_range.png", "upgrade_range");
    texture_manager->LoadTexture("assets/textures/ui/arrow.png", "arrow");

    //Menu
    texture_manager->LoadTexture("assets/textures/menu/background.png", "menu_bg");
    texture_manager->LoadTexture("assets/textures/menu/play_btn.png", "play_btn");
    texture_manager->LoadTexture("assets/textures/menu/play_btn_hover.png", "play_btn_hover");
    texture_manager->LoadTexture("assets/textures/menu/settings_btn.png", "settings_btn");
    texture_manager->LoadTexture("assets/textures/menu/settings_btn_hover.png", "settings_btn_hover");
    texture_manager->LoadTexture("assets/textures/menu/back_btn.png", "back_btn");
    texture_manager->LoadTexture("assets/textures/menu/back_btn_hover.png", "back_btn_hover");
    texture_manager->LoadTexture("assets/textures/menu/quit_btn.png", "quit_btn");
    texture_manager->LoadTexture("assets/textures/menu/quit_btn_hover.png", "quit_btn_hover");
    texture_manager->LoadTexture("assets/textures/menu/scores_btn.png", "scores_btn");
    texture_manager->LoadTexture("assets/textures/menu/scores_btn_hover.png", "scores_btn_hover");

    lettersTexture = texture_manager->GetTexture("letters");
    letterSrcRect = {0, 0, LETTER_WIDTH, LETTER_HEIGHT};

    // Resolution Buttons
    for (const auto& res : resolutions) {
        std::string resStr = std::to_string(res.first) + "x" + std::to_string(res.second);
        texture_manager->LoadTexture("assets/textures/menu/resolution_btn_" + resStr + ".png","resolution_btn_" + resStr);
        texture_manager->LoadTexture("assets/textures/menu/resolution_btn_hover_" + resStr + ".png","resolution_btn_hover_" + resStr);
    }


    upgradeMenuTexture = texture_manager->GetTexture("upgrade_menu");
    heartTexture = texture_manager->GetTexture("heart");
    numbersTexture = texture_manager->GetTexture("numbers");
    waveTextTexture = texture_manager->GetTexture("wave_text");
    gameOverTexture = texture_manager->GetTexture("game_over");
    upgradeFireTexture = texture_manager->GetTexture("upgrade_fire_rate");
    upgradeDamageTexture = texture_manager->GetTexture("upgrade_damage");
    upgradeRangeTexture = texture_manager->GetTexture("upgrade_range");
    menuBackground = texture_manager->GetTexture("menu_bg");
    buttonTexture = texture_manager->GetTexture("button");
    buttonHoverTexture = texture_manager->GetTexture("button_hover");
    collectibleTexture = texture_manager->GetTexture("collectible");


    player->ReloadTexture(*texture_manager, "player");
    if (ally) {
        ally->ReloadTexture(*texture_manager, "ally");
    }

    for (auto& enemy : enemyPool) {
        enemy->ReloadTexture(*texture_manager, "enemy1"); // Base enemy
    }

    for (auto& projectile : projectiles) {
        projectile->ReloadTexture(*texture_manager, "bullet");
    }
}
