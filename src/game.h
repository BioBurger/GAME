#include <iostream>
#include <ostream>
#include <stdio.h>
#include <SDL_image.h>
#include "GameObject.h"
#include "SDL2/SDL.h"
#include "Texture_Manager.h"
#include "Camera.h"
#include "TileMap.h"
#ifndef Game_h
#define Game_h

class Game {
private:
    Texture_Manager *texture_manager;
    bool isRunning;
    SDL_Window *window;
    SDL_Renderer *renderer;
    GameObject *player;
    Camera *camera;
    TileMap *tileMap;


public:
    Game();
    ~Game();
    void Init(const char* title, bool fullscreen);
    void HandleEvents();
    void Update(float deltaTime);
    void Render();
    void Clean();
    bool Running();

};

#endif //GAME_H
