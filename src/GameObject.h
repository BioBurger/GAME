#include <SDL_render.h>
#include "Texture_Manager.h"
#include <string>

class GameObject {
    private:
        Texture_Manager& manager;
        SDL_Texture* texture;
        float positionX;
        float positionY;
        SDL_Rect positionrect;
        float velocityX;
        float velocityY;
        SDL_Rect currentFrame;
        int frameWidth;
        int frameHeight;
        int currentFrameIndex;
        int totalFrames;
        float animationSpeed;
        float animationTimer;
        bool isAnimated;



    public:
    GameObject(Texture_Manager& manager, const std::string& textureID, int x, int y, int w, int h, int frameWidth, int frameHeight, int totalFrames, float animationSpeed);
    ~GameObject();
    void Update(float deltaTime);
    void Render(SDL_Renderer* renderer);
    SDL_Rect GetPosition()  const {return positionrect;}
    void SetPosition(int x, int y);;
    void SetVelocity(float vx, float vy);
};
