#include <SDL_render.h>
#include "Texture_Manager.h"
#include <string>

class GameObject {
    private:
        Texture_Manager& manager;
        SDL_Texture* texture;
        SDL_Rect positionrect;
        float velocityX;
        float velocityY;

    public:
    GameObject(Texture_Manager& manager, const std::string& textureID, int x, int y, int w, int h);
    ~GameObject();
    void Update(float Time);
    void Render(SDL_Renderer* renderer);
    SDL_Rect GetPosition()  const {return positionrect;}
    void SetPosition(int x, int y);;
};
