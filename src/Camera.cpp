#include "Camera.hpp"

Camera::Camera(int screenWidth, int screenHeight) {
    viewport = {0, 0, screenWidth, screenHeight};
    x = 0.0f;
    y = 0.0f;
}
void Camera::SetTarget(GameObject* newTarget) {
    target = newTarget;
    if(target) {
        Vector2f center = target->GetCenterPosition();
        x = center.x - viewport.w/2.0f;
        y = center.y - viewport.h/2.0f;

        viewport.x = static_cast<int>(x);
        viewport.y = static_cast<int>(y);
    }
}

void Camera::Update(float targetCenterX, float targetCenterY) {
    // Original implementation using floats
    const float SMOOTHING_FACTOR = 0.1f;
    float targetX = targetCenterX - viewport.w/2.0f;
    float targetY = targetCenterY - viewport.h/2.0f;

    x += (targetX - x) * SMOOTHING_FACTOR;
    y += (targetY - y) * SMOOTHING_FACTOR;

    // Clamping
    x = std::max(0.0f, std::min(x, static_cast<float>(mapWidth - viewport.w)));
    y = std::max(0.0f, std::min(y, static_cast<float>(mapHeight - viewport.h)));

    viewport.x = static_cast<int>(x);
    viewport.y = static_cast<int>(y);
}

void Camera::SetMapBounds(int width, int height) {
    mapWidth = width;
    mapHeight = height;
}
void Camera::Reset(int targetCenterX, int targetCenterY) {
    x = targetCenterX - viewport.w/2;
    y = targetCenterY - viewport.h/2;
    viewport.x = static_cast<int>(x);
    viewport.y = static_cast<int>(y);
}