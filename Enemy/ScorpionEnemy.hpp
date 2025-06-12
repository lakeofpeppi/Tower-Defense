//
// Created by Angelie Melisa on 2025/6/12.
//

#ifndef SCORPIONENEMY_HPP
#define SCORPIONENEMY_HPP
#pragma once
#include "Engine/Sprite.hpp"
#include <string>

class ScorpionEnemy : public Engine::Sprite {
public:
    bool hasSpoken = false;
    std::string targetSceneName;
    float animationTimer = 0.0f;
    float animationInterval = 1.0f;
    int animationFrame = 0;
    std::string baseImagePath;

    ScorpionEnemy(float x, float y, std::string baseImagePath, std::string targetSceneName);

    void Update(float deltaTime) override;
    void OnTouch(); // Just the declaration
};
#endif //SCORPIONENEMY_HPP
