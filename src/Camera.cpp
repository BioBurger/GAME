#include "Camera.h"

Camera::Camera(int screenWidth, int screenHeight) {
    viewport.w=screenWidth;
    viewport.h=screenHeight;
    viewport.x=0;
    viewport.y=0;
    mapWidth=INT_MAX;
    mapHeight=INT_MAX;
}

void Camera::Update(int targetX, int targetY, float delatTime) {
    float speedFactor = smoothSpeed * delatTime * 60;
    // Središče kamere na igralcu
    viewport.x += static_cast<int>((targetX-viewport.w / 2 - viewport.x) * speedFactor);
    viewport.y += static_cast<int>((targetY-viewport.h / 2 - viewport.y) * speedFactor);

    // Omeji kamero na meje mape
    viewport.x=std::max(0, std::min(viewport.x, mapWidth - viewport.w));
    viewport.y=std::max(0, std::min(viewport.y, mapHeight - viewport.h));
}
void Camera::WorldToScreen(int &x, int &y) {
    x -= viewport.x;
    y -=viewport.y;
}
