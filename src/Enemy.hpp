#pragma once
#include "GameObject.hpp"
#include "GameObject.hpp"

class Enemy : public GameObject {
private:
    GameObject* target;// Default player
    float moveSpeed = 100.0f;
    bool isActive = false;
    int level;

    const int ENEMY_FRAME_WIDTH = 32;
    const int ENEMY_FRAME_HEIGHT = 32;
    const int ENEMY_TOTAL_FRAMES = 5;
    const float ENEMY_ANIM_SPEED = 0.15f;


public:
    Enemy(Texture_Manager& manager,
          const std::string& textureName,
          int x, int y,
          int width, int height,
          int collisionWidth, int collisionHeight,
          int renderOrder,
          float speed,
          GameObject* target,
          int health);
    void Update(float deltaTime) override;
    void Revive(int x, int y, int level);
    void SetSpeed(float speed) { moveSpeed = speed; }
    float GetSpeed() const { return moveSpeed; }
    bool IsActive() const { return isActive; }
    void Render(SDL_Renderer* renderer, const SDL_Rect& cameraViewport) override;
    void SetTarget(GameObject* newTarget);
    int GetLevel() const {return level;}
    void ReloadTexture(Texture_Manager& tm, const std::string& baseTexture);
};