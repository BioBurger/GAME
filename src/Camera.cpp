#include "Camera.h"

Camera::Camera(int screenWidth, int screenHeight) {
    viewport.w=screenWidth;
    viewport.h=screenHeight;
    viewport.x=0;
    viewport.y=0;
    mapWidth=INT_MAX;
    mapHeight=INT_MAX;
}

void Camera::Update(int targetX, int targetY) {
    SDL_Log("Kamera: &d , &d | Igralec: &d , &d", viewport.x,viewport.y,targetX,targetY);
    // Središče kamere na igralcu
    int targetCenterX = targetX - viewport.w /2;
    int targetCenterY = targetY-viewport.h /2;
    //za za enakometren premik
    viewport.x +=static_cast<int>((targetCenterX-viewport.x) * smoothSpeed);
    viewport.y +=static_cast<int>((targetCenterY-viewport.y) * smoothSpeed);

    // Omeji kamero na meje mape
    viewport.x=std::max(0, std::min(viewport.x, mapWidth - viewport.w));
    viewport.y=std::max(0, std::min(viewport.y, mapHeight - viewport.h));
}
void Camera::WorldToScreen(int &x, int &y) {
    x -= viewport.x;
    y -=viewport.y;
}
