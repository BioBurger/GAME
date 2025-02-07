#include "Game.h"

Game::Game() {

}

Game::~Game() {

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
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        }
        isRunning = true;
    }
    else {
        isRunning = false;
    }

    std::cout << "NIGGER" << std::endl;
    std::cout << "NIGGER";

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
    SDL_RenderPresent(renderer);
}

bool Game::Running() {
    return isRunning;
}

void Game::Clean() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Game cleaned" << std::endl;
}