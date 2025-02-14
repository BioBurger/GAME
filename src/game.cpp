#include "Game.h"
#include <SDL2/SDL_keyboard.h>

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
        }
        SDL_Rect playerPos = player->GetPosition();
        camera->Update(playerPos.x + playerPos.w / 2, playerPos.y + playerPos.h / 2);
        tileMap->Update(deltaTime);
        player->SetVelocity(vx, vy);
        player->Update(deltaTime);
    }
}

void Game::Render() {
    SDL_RenderClear(renderer);
    tileMap->Render((renderer));//Rendera ozadje
    if (player) {
        player->Render(renderer);//rendera playerja
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