#include "Game.h"
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

            // Inicializiraj textureManager
            texture_manager = new Texture_Manager(renderer);
            texture_manager->LoadTexture("assets/textures/skibidi.png", "player");

            // Ustvari player objekt na sredini (primer: 64x64 velikost)
            int playerWidth = 64;
            int playerHeight = 64;
            int centerX = (1920 - playerWidth) / 2;
            int centerY = (1080 - playerHeight) / 2;
            player = new GameObject(*texture_manager, "player", centerX, centerY, playerWidth, playerHeight);
        }
        isRunning = true;
    }
    else {
        isRunning = false;
    }



}

void Game::HandleEvents() {
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type) {
        case SDL_QUIT:
            isRunning = false;
        break;

        default:
            break;
    }

}

void Game::Update() {

}

void Game::Render() {
    SDL_RenderClear(renderer);
    if (player) {
        player->Render(renderer);
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