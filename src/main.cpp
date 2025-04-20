#include <SDL2/SDL.h>
#include <cstdlib>
#include <time.h>
#include "game.hpp"

Game *game = nullptr;

int main(int argc, char* args[]) {
    srand(static_cast<unsigned>(time(nullptr)));

    game = new Game();
    game->Init("GAME", false);

    Uint32 previousTime = SDL_GetTicks();
    while(game->Running()) {
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - previousTime) / 1000.0f;
        previousTime = currentTime;

        game->HandleEvents();
        game->Update(deltaTime);
        game->Render();
    }

    game->Clean();
    return 0;
}
//    F1: Debug

//    F2: New wave

//    KP+: Speed +

//    KP-: Speed-


