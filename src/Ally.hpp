#pragma once
#include "GameObject.hpp"

class Ally : public GameObject {
public:
    Ally(Texture_Manager& tm, const std::string& textureID, int x, int y);
    void Update(float deltaTime) override;
    void Revive(int x, int y, int health);
    void ResetAnimation();
    enum class State { IDLE, MOVING };


private:
    State currentState = State::IDLE;
    State previousState = State::IDLE;
    Direction DetermineDirection();
};
#ifndef ALLY_H
#define ALLY_H

#endif //ALLY_H
