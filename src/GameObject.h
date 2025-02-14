#include <SDL_render.h>
#include "Texture_Manager.h"
#include <string>

class GameObject {
    private:
        Texture_Manager& manager;//texture
        SDL_Texture* texture;
        float positionX;//lokacija
        float positionY;
        SDL_Rect positionrect;
        float velocityX;
        float velocityY;
        SDL_Rect currentFrame;//animacija
        int frameWidth;
        int frameHeight;
        int currentFrameIndex;
        int totalFrames;
        float animationSpeed;
        float animationTimer;
        bool isAnimated;
        enum class State {IDLE, MOVING};//stanja animacije (popravek od učer)
        enum class Direction {LEFT, RIGHT, UP, DOWN};
        State currentState;
        Direction currentDirection;
        SDL_RendererFlip flipType;//za levo(i hate not fliped sprites)
        int animationRow;//1. idle front||2. idle right||3. idle front(ignoreamo)||4. idle back||5. moving front||6. moving back||7. moving right ||8. death
        float idleAnimationSpeed;
        float moveAnimationSpeed;





    public:
    GameObject(Texture_Manager& manager, const std::string& textureID, int x, int y, int w, int h, int frameWidth, int frameHeight, int totalFrames, float animationSpeed);
    ~GameObject();
    void Update(float deltaTime);
    void Render(SDL_Renderer* renderer);
    SDL_Rect GetPosition()  const {return positionrect;}
    void SetPosition(int x, int y);;
    void SetVelocity(float vx, float vy);
};
