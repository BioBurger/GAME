#include <iostream>
#include <ostream>
#include <stdio.h>
#include <SDL_image.h>
#include "SDL2/SDL.h"

#ifndef Game_h
#define Game_h

class Game {
private:
    bool isRunning;
    SDL_Window *window;
    SDL_Renderer *renderer;


public:
    Game();
    ~Game();

    void Init(const char* title, bool fullscreen);
    void HandleEvents();

    void Update();
    void Render();
    void Clean();

    bool Running();

};

#endif //GAME_H
