#include "Game.hpp"
#include <SDL2/SDL_keyboard.h>
#include <random>
#include "Projectile.hpp"
#include "Enemy.hpp"

Game::Game() :texture_manager(NULL)  {
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
    for (auto& p : projectiles) {
        delete p;
    }
    projectiles.clear();
    SDL_DestroyTexture(waveTextTexture);
    SDL_DestroyTexture(numbersTexture);

};

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

            // Player
            if (!texture_manager->HasTexture("player")) {
                if(!texture_manager->LoadTexture("assets/textures/Player/Turtle-Wick.png", "player")) {
                    SDL_Log("NAPACNA POT DO PLAYER TEKSTURE!");
                    exit(1);
                }
            }
            // Water
            if (!texture_manager->HasTexture("water")) {
                texture_manager->LoadTexture("assets/textures/Tiles/water_sheet.png", "water");
            }
            // Enemys
            if (!texture_manager->HasTexture("enemy")) {
                if (!texture_manager->LoadTexture("assets/textures/Enemys/Enemy1.png", "enemy")) {
                    SDL_Log("NAPACNA POT DO ENEMYJA TEKSTURE!");
                    exit(1);
                }
            }
            // Game Over
            if (!texture_manager->HasTexture("game_over")) {
                if(!texture_manager->LoadTexture("assets/textures/Screens/GameOver.png", "game_over")) {
                    SDL_Log("MANJKA GAME OVER TEKSTURA!");
                    exit(1);
                }
            }
            // UI elementi
            if (!texture_manager->HasTexture("heart")) {
                if(!texture_manager->LoadTexture("assets/textures/ui/Heart.png", "heart")) {
                    SDL_Log("MANJKA HEART TEKSTURA!");
                    exit(1);
                }
            }
            if (!texture_manager->HasTexture("bullet")) {
                if (!texture_manager->LoadTexture("assets/textures/Weapons/bullet.png", "bullet")) {
                    SDL_Log("MANJKA METEK TEKSTURA!");
                    exit(1);
                }
            }
            // Števile za wave
            if (!texture_manager->HasTexture("numbers")) {
                if(!texture_manager->LoadTexture("assets/textures/ui/numbers.png", "numbers")) {
                    SDL_Log("MANJKA NUMBER TEKSTURA!");
                    exit(1);
                }
            }
            // Wave napis
            if (!texture_manager->HasTexture("wave_text")) {
                if(!texture_manager->LoadTexture("assets/textures/ui/wave.png", "wave_text")) {
                    SDL_Log("MANJKA WAVE TEKSTURA!");
                    exit(1);
                }
            }
            // Reference v mapi
            heartTexture = texture_manager->GetTexture("heart");
            numbersTexture = texture_manager->GetTexture("numbers");
            waveTextTexture = texture_manager->GetTexture("wave_text");
            gameOverTexture = texture_manager->GetTexture("game_over");

            betweenWaves = true;  // settingi za wave
            waveTimer = 0.0f;
            currentWave = 0;


            // Player ustvarjen
            player = new GameObject(*texture_manager,"player",PLAYER_SPAWN_X, PLAYER_SPAWN_Y,playerWidth, playerHeight,frameWidth, frameHeight,totalFrames, animationSpeed);
            // Komponente
            camera = new Camera(1920, 1080);
            camera->Update(PLAYER_SPAWN_X + playerWidth/2, PLAYER_SPAWN_Y + playerHeight/2);
            camera->SetMapBounds(20000, 20000);
            tileMap = new TileMap(*texture_manager, *camera);

            InitializeEnemyPool(20);//Koliko enemijev na wave
            isRunning = true;
        }
    } else {
        isRunning = false;
        SDL_Log("NAPACNA INICIALIZACIJA SDL: %s", SDL_GetError());
    }

}


void Game::HandleEvents() {
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        if(event.type == SDL_QUIT) isRunning = false;
        if(gameOver && event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_r) {
            RestartGame();
        }
        if (event.type == SDL_KEYDOWN) {
            switch(event.key.keysym.sym) {
                case SDLK_F1:
                    debugMode = !debugMode;
                break;
                case SDLK_F2:
                    StartNewWave();
                break;
                case SDLK_KP_PLUS:
                    player->SetSpeedMultiplier(1.5f);
                break;
                case SDLK_KP_MINUS:
                    player->SetSpeedMultiplier(0.75f);
                break;
            }
        }
    }
}

void Game::Update(float deltaTime) {
    if (gameOver || !player->IsAlive()) {
        return;
    }

    // Updajtanje dmg cooldowna
    if (player->GetDamageCooldown() > 0.0f) {
        player->SetDamageCooldown(player->GetDamageCooldown() - deltaTime);
    }

    // Updati za objekte
    UpdateWave(deltaTime);
    ProcessInput();
    UpdatePlayer(deltaTime);
    UpdateEnemies(deltaTime);

    // Update metki
    for (auto it = projectiles.begin(); it != projectiles.end();) {
        (*it)->Update(deltaTime);

        // Check for hits
        if (CheckCollision((*it)->GetCollisionBox(),(*it)->GetTarget()->GetCollisionBox())) {
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

    if (!player->IsAlive()) {
        gameOver = true;
    }
}
// Collisoni
bool Game::CheckCollision(const SDL_Rect& a, const SDL_Rect& b) {
    bool xOverlap = (a.x <b.x + b.w) && (a.x + a.w > b.x);
    bool yOverlap = (a.y <b.y + b.h) && (a.y + a.h > b.y);

    return xOverlap && yOverlap;
}

void Game::Render() {
    SDL_RenderClear(renderer);
    for (auto& enemy : enemies) {
        SDL_Rect pos = enemy->GetPosition();
        SDL_Log("Enemy position: [%d, %d]", pos.x, pos.y);
        SDL_Rect cameraViewport = camera->GetViewport();
        enemy->Render(renderer, cameraViewport);
    }

    if( !gameOver ) {
        // Podaj kamero vsem elementom
        SDL_Rect cameraViewport = camera->GetViewport();
        tileMap->Render(renderer, cameraViewport);
        player->Render(renderer, cameraViewport);

        for (auto& enemy : enemies) {
            enemy->Render(renderer, cameraViewport);
        }
    }else {
        //gameover
        SDL_Rect screenRect = {0, 0, 1920, 1080};
        SDL_RenderCopy(renderer, gameOverTexture, NULL, &screenRect);
    }
    // Render metki
    for (auto& projectile : projectiles) {
        projectile->Render(renderer, camera->GetViewport());
    }
    // RENDER WAVE ŠTEVILKE
    if (!gameOver && !betweenWaves) {
        RenderWaveNumber();
    }
    // Health bar
    int startX = 50;
    int startY = 50;
    int spacing = 10;
    int currentHealth = player->GetHealth();
    int maxHearts = player->GetMaxHealth() / HEALTH_PER_HEART;

    int fullHearts = currentHealth / HEALTH_PER_HEART;
    int remainder = currentHealth % HEALTH_PER_HEART;

    for (int i = 0; i < maxHearts; ++i) {
        int phase;

        if (i < fullHearts) {
            phase = 0; // Polno srce
        } else if (i == fullHearts && remainder > 0) {
            // Delno srce z zaokroževanjem
            float ratio = static_cast<float>(remainder) / HEALTH_PER_HEART;
            int originalPhase = static_cast<int>(std::round(ratio * (HEART_PHASES - 1)));
            phase = HEART_PHASES - 1 - originalPhase;
        } else {
            phase = HEART_PHASES - 1; // Prazno srce
        }

        SDL_Rect srcRect = {phase * (112 / HEART_PHASES),0,112 / HEART_PHASES,srcHeartSize};

        SDL_Rect destRect = {startX + (destHeartSize + spacing) * i,startY,destHeartSize,destHeartSize};

        SDL_RenderCopy(renderer, heartTexture, &srcRect, &destRect);
        if (debugMode) {
            SDL_Log("Debug info - Sovražniki: %d, Val: %d", enemies.size(), currentWave);
        }
    }


    if (debugMode) {
        // DEBUG:
        SDL_Rect cameraViewport = camera->GetViewport();
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

        // Igralec
        SDL_Rect playerBox = player->GetCollisionBox();
        playerBox.x -= cameraViewport.x;
        playerBox.y -= cameraViewport.y;
        SDL_RenderDrawRect(renderer, &playerBox);

        // Sovražniki
        for(auto& enemy : enemies) {
            SDL_Rect enemyBox = enemy->GetCollisionBox();
            enemyBox.x -= cameraViewport.x;
            enemyBox.y -= cameraViewport.y;
            SDL_RenderDrawRect(renderer, &enemyBox);
        }
    }
    SDL_RenderPresent(renderer);

}

bool Game::Running() {
    return isRunning;
}

void Game::Clean() {
    delete player;
    delete texture_manager;
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Game cleaned" << std::endl;
}

void Game::SpawnEnemy() {
    const int MIN_SPAWN_DISTANCE = 500;  // Odaljenost spawna
    const int MAX_SPAWN_DISTANCE = 800;

    SDL_Rect playerPos = player->GetPosition();

    // Random kot
    float angle = static_cast<float>(rand() % 360) * (M_PI / 180.0f);

    // Random odaljenost
    float distance = MIN_SPAWN_DISTANCE + static_cast<float>(rand()) / RAND_MAX * (MAX_SPAWN_DISTANCE - MIN_SPAWN_DISTANCE);

    int spawnX = playerPos.x + static_cast<int>(cos(angle) * distance);
    int spawnY = playerPos.y + static_cast<int>(sin(angle) * distance);

    // Znotraj mape
    spawnX = std::clamp(spawnX, 0, 20000);
    spawnY = std::clamp(spawnY, 0, 20000);

    // Ga damo v vektor
    Enemy* e = GetPooledEnemy();
    e->Revive(spawnX, spawnY);
    e->GetHealth(50);
    enemies.push_back(e);
}
void Game::RestartGame() {
    // Reset player
    delete player;
    player = new GameObject(*texture_manager, "player",PLAYER_SPAWN_X,PLAYER_SPAWN_Y,playerWidth,playerHeight,frameWidth,frameHeight,totalFrames,animationSpeed);

    // Reset enemy
    enemies.clear();
    for (auto& e : enemyPool) {
        e->SetTarget(player);
        e->Revive(-1000, -1000); // Move off-screen instead of TakeDamage
    }

    // Reset camera + state + wave
    camera->Reset(PLAYER_SPAWN_X + playerWidth/2, PLAYER_SPAWN_Y + playerHeight/2);
    gameOver = false;
    currentWave = 0;
    betweenWaves = true;
}
void Game::StartNewWave() {
    currentWave++;
    enemiesPerWave = 20;
    enemiesRemaining = enemiesPerWave;
    betweenWaves = false;

    // Ohranjam enemyje
    for (auto& e : enemyPool) {
        e->Revive(-1000, -1000); // Začasno skrij
    }

    // Debug
    SDL_Log("----- ZAČETEK VALA %d -----", currentWave);
}
void Game::RenderWaveNumber() {
    if (!waveTextTexture || !numbersTexture) return;
    if (currentWave != cachedWaveNumber) {
        SDL_DestroyTexture(cachedWaveTexture);
        cachedWaveTexture = CreateWaveNumberTexture(); // Uporabite PRAVILNO ime
        cachedWaveNumber = currentWave;
    }

    // Render če obstaja
    if (cachedWaveTexture) {
        // Določi pozicijo v zgornjem desnem kotu
        int texWidth, texHeight;
        SDL_QueryTexture(cachedWaveTexture, NULL, NULL, &texWidth, &texHeight);
        SDL_Rect destRect = {
            1920 - texWidth - 50, // X pozicija (zaslon širina - tekstura širina - rob)
            50,                   // Y pozicija
            texWidth,             // Širina
            texHeight             // Višina
        };
        SDL_RenderCopy(renderer, cachedWaveTexture, NULL, &destRect);
    }
}
// Shranjevanje nasprotnikov
void Game::InitializeEnemyPool(int initialSize) {
    for (int i = 0; i < initialSize; ++i) {
        Enemy* e = new Enemy(
            *texture_manager,
            "enemy",
            0, 0,
            player,
            50
        );
        e->Revive(-1000, -1000); // Izven screna
        enemyPool.push_back(e);
    }
}
// Vzemanje iz poola
Enemy* Game::GetPooledEnemy() {
    for (auto& e : enemyPool) {
        if (!e->IsAlive()) {
            e->Revive(0, 0);
            e->SetTarget(player);
            return e;
        }
    }

    Enemy* newEnemy = new Enemy(*texture_manager,"enemy",0,0,player,50);
    newEnemy->SetTarget(player);
    enemyPool.push_back(newEnemy);
    return newEnemy;
}
void Game::UpdateWave(float deltaTime) {
    // Delay med wavi
    if (betweenWaves) {
        waveTimer += deltaTime;
        SDL_Log("Čakanje na val... Timer: %.1f/%.1f", waveTimer, waveStartDelay); // Debug

        if (waveTimer >= waveStartDelay) {
            StartNewWave();
            waveTimer = 0.0f;
        }
    } else {
        if (enemiesRemaining <= 0 && enemies.empty()) {
            betweenWaves = true;
        }
    }
}
void Game::UpdateEnemies(float deltaTime) {
    // Enemy se ne spawna med wavi
    if (!betweenWaves && enemiesRemaining > 0) {
        spawnTimer += deltaTime;
        if (spawnTimer >= SPAWN_INTERVAL) {
            SpawnEnemy();
            enemiesRemaining--;
            spawnTimer = 0.0f;
        }
    }

    // Enemy update + collisioni
    for (auto it = enemies.begin(); it != enemies.end();) {
        Enemy* enemy = *it;
        enemy->Update(deltaTime);

        // Ali imata iste kordinate hit box
        if (CheckCollision(enemy->GetCollisionBox(), player->GetCollisionBox())) {
            // Damage player
            if (player->GetDamageCooldown() <= 0.0f) {
                player->TakeDamage(5);
                player->SetDamageCooldown(player->GetDamageCooldownTime());
            }

            // Dmg enemy
            enemy->TakeDamage(50);
        }

        // Odstrani a ne izbriše
        if (!enemy->IsAlive()) {
            it = enemies.erase(it);
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
        // Posodobi pozicijo igralca
        player->Update(deltaTime);

        // Pridobi trenutno pozicijo igralca
        SDL_Rect playerPos = player->GetPosition();

        // zračunaj središče igralca
        int playerCenterX = playerPos.x + playerPos.w / 2;
        int playerCenterY = playerPos.y + playerPos.h / 2;

        // Posodobi kamero na središče igralca
        camera->Update(playerCenterX, playerCenterY);

        // Posodobi tilemapo
        tileMap->Update(deltaTime);

        // Streljanje
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
    SDL_Log("Total enemies: %zu", enemies.size());

    for (auto& enemy : enemies) {
        Vector2f enemyPos = enemy->GetCenterPosition();
        float distance = sqrt(pow(enemyPos.x - playerPos.x, 2) +
                            pow(enemyPos.y - playerPos.y, 2));
        SDL_Log("Checking enemy at (%f, %f), Player at (%f, %f), Distance: %f",
enemyPos.x, enemyPos.y, playerPos.x, playerPos.y, distance);
        if (distance < minDistance) {
            minDistance = distance;
            nearest = enemy;


        }
    }


    return nearest;
}
void Game::ShootProjectile(Enemy* target) {
    if (!target) return; // Ensure target is valid

    float startX = player->GetPosition().x;
    float startY = player->GetPosition().y;


    if(target) {
        SDL_Log("Buttle fired");
    }
    projectiles.push_back(new Projectile(*texture_manager, target, startX, startY, PROJECTILE_SPEED, PROJECTILE_SPEED, PISTOL_RANGE));
}


// Premikanje
void Game::ProcessInput() {
    const Uint8* keystate = SDL_GetKeyboardState(NULL);
    float hitrost = 400.0f;// Hitrost px
    float vx = 0, vy = 0;
    if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LSHIFT]) {
        hitrost *= 1.5f; //Šprint
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
    // Naloži sliko ker je ttf - šupak
    SDL_Surface* waveSurface = IMG_Load("assets/textures/ui/wave.png");
    if (!waveSurface) {
        SDL_Log("Napaka pri nalaganju wave.png: %s", IMG_GetError());
        return nullptr;
    }

    // Pripravi površino
    SDL_Surface* combinedSurface = SDL_CreateRGBSurfaceWithFormat(0,waveSurface->w + (std::to_string(currentWave).length() * (NUMBER_WIDTH + 2)),std::max(waveSurface->h, NUMBER_HEIGHT),32,SDL_PIXELFORMAT_RGBA32);

    // Prekopiraj sloko wave
    SDL_Rect waveDest = {0, (combinedSurface->h - waveSurface->h)/2, waveSurface->w, waveSurface->h};
    SDL_BlitSurface(waveSurface, NULL, combinedSurface, &waveDest);
    SDL_FreeSurface(waveSurface);

    // Naloži števke iz datoteke
    SDL_Surface* numbersSurface = IMG_Load("assets/textures/ui/numbers.png");
    if (!numbersSurface) {
        SDL_Log("Napaka pri nalaganju numbers.png: %s", IMG_GetError());
        SDL_FreeSurface(combinedSurface);
        return nullptr;
    }

    // Dodaj števke
    int xOffset = waveDest.w;
    std::string waveStr = std::to_string(currentWave);
    for (char c : waveStr) {
        int digit = c - '0';
        SDL_Rect srcRect = {digit * NUMBER_WIDTH, 0, NUMBER_WIDTH, NUMBER_HEIGHT};
        SDL_Rect destRect = {xOffset, (combinedSurface->h - NUMBER_HEIGHT)/2, NUMBER_WIDTH, NUMBER_HEIGHT};
        SDL_BlitSurface(numbersSurface, &srcRect, combinedSurface, &destRect);
        xOffset += NUMBER_WIDTH + 2;
    }
    SDL_FreeSurface(numbersSurface);

    // Ustvari teksturo iz površine
    SDL_Texture* result = SDL_CreateTextureFromSurface(renderer, combinedSurface);
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
