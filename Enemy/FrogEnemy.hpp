//
// Created by Angelie Melisa on 2025/6/13.
//

#ifndef LIZARDENEMY_HPP
#define LIZARDENEMY_HPP
#pragma once
#include "Engine/Sprite.hpp"
#include <string>

class FrogEnemy : public Engine::Sprite {
public:
    bool hasSpoken = false;
    std::string targetSceneName;
    float animationTimer = 0.0f;
    float animationInterval = 0.3f;
    int animationFrame = 0;
    std::string baseImagePath;
    bool isdisappearing = false;
    int deathFrame = 0;
    float deathAnimationInterval = 0.3f;
    FrogEnemy(float x, float y, std::string baseImagePath, std::string targetSceneName);

    void Update(float deltaTime) override;
    void OnTouch(); // Just the declaration
};
#endif //LIZARDENEMY_HPP
