#pragma once
#include <SDL_render.h>
#include "Texture_Manager.h"
#include <string>
#include <SDL.h>

class GameObject {
public:
    GameObject(Texture_Manager& manager, const std::string& textureID, int x, int y, int w, int h, int frameWidth, int frameHeight, int totalFrames, float animationSpeed, int startHealth = 100);
    ~GameObject();
    enum class State {IDLE, MOVING, DEAD};//stanja animacije + ani je mrtev
    enum class Direction {LEFT, RIGHT, UP, DOWN};
    virtual void Update(float deltaTime);
    void Render(SDL_Renderer* renderer, const SDL_Rect& cameraViewport);
    SDL_Rect GetPosition()  const {return positionrect;}
    void SetPosition(int x, int y);;
    void SetVelocity(float vx, float vy);
    void SetDirection(Direction newDir);
    Direction GetDirection()const;
    int GetHealt() const{return health;}
    void TakeDamage(int damage);
    bool IsAlive() const { return health > 0; }
    void PlayDeathAnimation();
protected:
    int health;
    int maxHealth;
    SDL_Rect positionrect;
    float velocityX;
    float velocityY;
    Texture_Manager& manager;//texture
    SDL_Texture* texture;
    float positionX;//lokacija
    float positionY;
    SDL_Rect currentFrame;//animacija
    int frameWidth;
    int frameHeight;
    int currentFrameIndex;
    int totalFrames;
    float animationSpeed;
    float animationTimer;
    bool isAnimated;
    State currentState;
    Direction currentDirection;
    SDL_RendererFlip flipType;//za levo(i hate not fliped sprites)
    int animationRow;//1. idle front||2. idle right||3. idle front(ignoreamo)||4. idle back||5. moving front||6. moving back||7. moving right ||8. death
    float idleAnimationSpeed;
    float moveAnimationSpeed;



};