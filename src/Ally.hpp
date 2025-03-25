#pragma once
#include "GameObject.hpp"

class Ally : public GameObject {
public:
    Ally(Texture_Manager& manager, const std::string& textureID, int x, int y);
    void Update(float deltaTime) override;

private:
    enum class State { IDLE, MOVING };
    State currentState = State::IDLE;
    State previousState = State::IDLE;
    GameObject::Direction DetermineDirection();
};
#ifndef ALLY_H
#define ALLY_H

#endif //ALLY_H
