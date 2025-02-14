#include "Camera.h"

Camera::Camera(int screenWidth, int screenHeight) {
    viewport = {0, 0, screenWidth, screenHeight};
}

void Camera::Update(int targetCenterX, int targetCenterY) {
    // Centriraj kamero na igralca
    viewport.x = targetCenterX - viewport.w / 2;
    viewport.y = targetCenterY - viewport.h / 2;

    // Prepreči izhod iz meja
    viewport.x = std::max(0, std::min(viewport.x, mapWidth - viewport.w));
    viewport.y = std::max(0, std::min(viewport.y, mapHeight - viewport.h));
}

void Camera::SetMapBounds(int width, int height) {
    mapWidth = width;
    mapHeight = height;
}