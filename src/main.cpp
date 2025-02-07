#include <SDL2/SDL.h>
#include <cstdlib>
#include <time.h>
#include "game.h"

Game *game = nullptr;

int main(int argumentCount, char *arguments[]){
    srand(time(NULL));

    game = new Game();

    game->Init("GAME", false);

    while (game->Running()) {
        game->HandleEvents();
        game->Update();
        game->Render();
    }

    game->Clean();

    return 0;
}
