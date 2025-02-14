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
            //Inicializiram camero in tileMap
            camera = new Camera(1920, 1080);
            tileMap = new TileMap(*texture_manager, *camera);
            //Nastavim FrameRate za sprite sheet
            int playerWidth = 64;
            int playerHeight = 64;
            int frameWidth = 16;
            int frameHeight = 16;
            int totalFrames = 8;
            float animationSpeed = 0.1f;
            // Ustvarim player objekt na sredini
            int centerX = (1920 - playerWidth) / 2;
            int centerY = (1080 - playerHeight) / 2;
            player = new GameObject(*texture_manager, "player", centerX, centerY, playerWidth, playerHeight, frameWidth, frameHeight, totalFrames, animationSpeed);
        }
        isRunning = true;
    }
    else {
        isRunning = false;
    }



}

void Game::HandleEvents() {
    SDL_PumpEvents();
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            isRunning = false;
        }
    }
}

void Game::Update(float deltaTime) {
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
            std::cout <<"Spawn enemy"<<std::endl;
            SpawnEnemy();
            spawnTimer = 0.0f;
        }
        //update enemy
        for (auto it = enemies.begin(); it != enemies.end();) {
            if (!(*it)->IsAlive()){
                std::cout<<"Enemy died"<<std::endl;
                delete *it;
                it = enemies.erase(it);
            }else {
                (*it)->Update(deltaTime);
                ++it;
            }
        }
        for (auto& enemy : enemies) {
            if (CheckCollision(player->GetPosition(), enemy->GetPosition())) {
                player->TakeDamage(5);
                enemy->TakeDamage(50);
            }
        }
    }
}
bool Game::CheckCollision(const SDL_Rect& a, const SDL_Rect& b) {
    return (a.x < b.x + b.w && a.x < a.w + b.x && a.y < b.y + b.h && a.y + a.h > b.y);
}

void Game::Render() {
    SDL_RenderClear(renderer);

    // Podaj kamero vsem elementom
    SDL_Rect cameraViewport = camera->GetViewport();
    tileMap->Render(renderer, cameraViewport);
    player->Render(renderer, cameraViewport);

    for (auto& enemy : enemies) {
        enemy->Render(renderer, cameraViewport);
    }

    // Health bar (ostane na fiksnem mestu)
    SDL_Rect healthBar = {50, 50, player->GetHealt() * 2, 20};
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &healthBar);

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
    std::cout<<"Enemy spawn: "<<spawnX<<","<<spawnY<<std::endl;

    enemies.push_back(new Enemy(*texture_manager, "enemy", spawnX, spawnY, player, 50));
}