#pragma once
#include <SDL_render.h>
#include "Texture_Manager.hpp"
#include <string>
#include <SDL.h>
#include "Vector2f.hpp"

class GameObject {
public:
    GameObject(Texture_Manager& manager, const std::string& textureID, int x, int y, int w, int h, int frameWidth, int frameHeight, int totalFrames, float animationSpeed, int startHealth = 100);
    ~GameObject();
    enum class State {IDLE, MOVING, DEAD};//stanja animacije + ani je mrtev
    enum class Direction {LEFT, RIGHT, UP, DOWN};
    virtual void Update(float deltaTime);
    virtual void Render(SDL_Renderer* renderer, const SDL_Rect& cameraViewport);
    SDL_Rect GetPosition()  const {return positionrect;}
    void SetPosition(int x, int y);;
    void SetVelocity(float vx, float vy);
    void SetDirection(Direction newDir);
    Direction GetDirection()const;
    int GetHealth() const{return health;}
    void TakeDamage(int damage);
    bool IsAlive() const { return health > 0; }
    void PlayDeathAnimation();
    SDL_Rect GetCollisionBox() const;
    int GetMaxHealth() const {return maxHealth;}
    void SetSpeedMultiplier(float mult) { speedMultiplier = mult; }
    float GetVelocityX() const { return velocityX; }
    float GetVelocityY() const { return velocityY; }
    float GetDamageCooldown() const { return damageCooldown; }
    void SetDamageCooldown(float time) { damageCooldown = time; }
    float GetDamageCooldownTime() const { return DAMAGE_COOLDOWN_TIME; }
    Vector2f GetCenterPosition() const;
    Vector2f position;
    void ReloadTexture(Texture_Manager& tm, const std::string& textureID);
    void SetHealth(int newHealth) { health = newHealth; }
    int GetWidth() const { return width; }
    int GetHeight() const { return height; }
    void SetActive(bool active) { isActive = active; }
    bool IsActive() const { return isActive; }
protected:
    bool isActive = true;
    int health;
    int maxHealth;
    int width;
    int height;
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
    int collisionOffsetX = 16;//za sredinski hit box
    int collisionOffsetY = 16;
    int collisionWidth = 48;
    int collisionHeight = 48;
    float speedMultiplier = 1.0f;
    bool useVerticalAnimation = true;
private:
    float damageCooldown = 0.0f;
    const float DAMAGE_COOLDOWN_TIME = 0.0f;
};