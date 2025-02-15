#include "Game.h"
#include <SDL2/SDL_keyboard.h>
#include <random>

#include "Enemy.h"

Game::Game() :texture_manager(NULL)  {
    texture_manager=NULL;
    player=NULL;
}

Game::~Game() {
    delete player;
    delete texture_manager;
};

void Game::Init(const char* title, bool fullscreen) {


    int flags = 0;
    if (fullscreen) {
        flags = SDL_WINDOW_FULLSCREEN;
    }

    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {

        window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920, 1080, flags);

        renderer = SDL_CreateRenderer(window, -1, 0);
        if (renderer) {
            SDL_SetRenderDrawColor(renderer, 50, 0, 0, 0);
            // Inicializira textureManager
            texture_manager = new Texture_Manager(renderer);
            texture_manager->LoadTexture("assets/textures/Player/Turtle-Wick.png", "player");
            texture_manager->LoadTexture("assets/textures/Tiles/water_sheet.png", "water");
            texture_manager->LoadTexture("assets/textures/Enemys/Enemy1.png", "enemy");
            texture_manager->LoadTexture("assets/textures/Screens/GameOver.png", "game_over");
            texture_manager->LoadTexture("assets/textures/Extra/Heart.png", "heart");
            heartTexture = texture_manager->GetTexture("heart");
            gameOverTexture = texture_manager->GetTexture("game_over");
            //Inicializiram camero in tileMap
            camera = new Camera(1920, 1080);
            tileMap = new TileMap(*texture_manager, *camera);
            //Nastavim FrameRate za sprite sheet
            // Ustvarim player objekt na sredini
            player = new GameObject(*texture_manager, "player", PLAYER_SPAWN_X, PLAYER_SPAWN_Y, playerWidth, playerHeight, frameWidth, frameHeight, totalFrames, animationSpeed);
        }
        isRunning = true;
    }
    else {
        isRunning = false;
    }



}

void Game::HandleEvents() {
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        if(event.type == SDL_QUIT) isRunning = false;

        if(gameOver && event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_r) {
            RestartGame();
        }
    }
}

void Game::Update(float deltaTime) {
    if (gameOver || !player->IsAlive()) {
        return;
    };
    if (player) {
        const Uint8* keystate = SDL_GetKeyboardState(NULL);
        float hitrost = 200.0f;
        float vx = 0, vy = 0;

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
            }else {
                player->SetDirection(vy > 0 ?  GameObject::Direction::DOWN : GameObject::Direction::UP);
            }
        }
        SDL_Rect playerPos = player->GetPosition();
        int playerCenterX = playerPos.x + playerPos.w / 2;
        int playerCenterY = playerPos.y + playerPos.h / 2;
        camera->Update(playerCenterX, playerCenterY);
        tileMap->Update(deltaTime);
        player->SetVelocity(vx, vy);
        player->Update(deltaTime);

        //spawn enemy
        spawnTimer += deltaTime;
        if (spawnTimer >= SPAWN_INTERVAL) {
            SpawnEnemy();
            spawnTimer = 0.0f;
        }
        //update enemy
        for (auto it = enemies.begin(); it != enemies.end();) {
            if (!(*it)->IsAlive()){
                delete *it;
                it = enemies.erase(it);
            }else {
                (*it)->Update(deltaTime);
                ++it;
            }
        }
        for (auto& enemy : enemies) {
            if (CheckCollision(
                player->GetCollisionBox(),
                enemy->GetCollisionBox()
            )) {
                player->TakeDamage(5);
                enemy->TakeDamage(50);
            }
        }
        if (!player->IsAlive()) {
            gameOver = true;
            return;
        }
    }
}
bool Game::CheckCollision(const SDL_Rect& a, const SDL_Rect& b) {
    bool xOverlap = (a.x <b.x + b.w) && (a.x + a.w > b.x);
    bool yOverlap = (a.y <b.y + b.h) && (a.y + a.h > b.y);

    return xOverlap && yOverlap;
}

void Game::Render() {
    SDL_RenderClear(renderer);


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
    // Health bar
    int currentHealth = player->GetHealth();
    int fullHearts = currentHealth / 20;//hp na srček
    //kje so hearti + utripanje
    int startX = 50;
    int startY= 50;
    int spacing = 10;

    Uint32 currentTicks = SDL_GetTicks();

    for (int i= 0; i< maxHearts; i++ ) {
        SDL_Rect destRect = { startX + ( destHeartSize + spacing ) * i, startY, destHeartSize, destHeartSize};
        SDL_Rect srcRect = {0, 0, srcHeartSize, srcHeartSize};

        if (i >= fullHearts) {
            srcRect.x = srcHeartSize;
        }

        if (currentHealth <= 20 && i== fullHearts -1 ) {
            srcRect.x = (SDL_GetTicks() % 400 < 200) ? 0: destHeartSize;
        }

        SDL_SetTextureColorMod(heartTexture, 255, 255, 255);
        SDL_RenderCopy(renderer, heartTexture, &srcRect, &destRect);
    }
    // DEBUG: Prikaz kolizijskih okvirjev (prilagojeno za kamero)
    SDL_Rect cameraViewport = camera->GetViewport();
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    // Igralec
    SDL_Rect playerBox = player->GetCollisionBox();
    playerBox.x -= cameraViewport.x; // Prilagodi za kamero
    playerBox.y -= cameraViewport.y;
    SDL_RenderDrawRect(renderer, &playerBox);

    // Sovražniki
    for(auto& enemy : enemies) {
        SDL_Rect enemyBox = enemy->GetCollisionBox();
        enemyBox.x -= cameraViewport.x; // Prilagodi za kamero
        enemyBox.y -= cameraViewport.y;
        SDL_RenderDrawRect(renderer, &enemyBox);
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
    //izven kamere
    SDL_Rect viewport = camera->GetViewport();
    std::random_device rd;
    std::mt19937 gen(rd());

    //random stran
    std::uniform_real_distribution<> sideDist(0, 3);
    int side = sideDist(gen);

    int spawnX, spawnY;
    const int SPAWN_OFFSET = 500;
    switch (side) {
        case 0://levo
                spawnX = viewport.x - SPAWN_OFFSET;
                spawnY = viewport.y + (rand() % viewport.h);
        break;
        case 1://desno
            spawnX = viewport.x + viewport.w + SPAWN_OFFSET;
            spawnY = viewport.y + (rand() % viewport.h);
        break;
        case 2://zgoraj
            spawnX = viewport.x + (rand() % viewport.w);
            spawnY = viewport.y - SPAWN_OFFSET;
        break;
        case 3://spodaj
            spawnX = viewport.x + (rand() % viewport.w);
            spawnY = viewport.y + viewport.h + SPAWN_OFFSET;
        break;
    }

    enemies.push_back(new Enemy(*texture_manager, "enemy", spawnX, spawnY, player, 50));
}
void Game::RestartGame() {
    delete player;
    player = new GameObject(*texture_manager, "player", PLAYER_SPAWN_X, PLAYER_SPAWN_Y, playerWidth, playerHeight, frameWidth, frameHeight, totalFrames, animationSpeed);
    camera->Update(PLAYER_SPAWN_X + playerWidth/2, PLAYER_SPAWN_Y + playerHeight/2);
    enemies.clear();
    gameOver = false;
    spawnTimer = 0.0f;
}

