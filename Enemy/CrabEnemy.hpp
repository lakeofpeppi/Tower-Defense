//
// Created by Angelie Melisa on 2025/6/13.
//

#ifndef CRABENEMY_HPP
#define CRABENEMY_HPP
#pragma once
#include "Engine/Sprite.hpp"
#include <string>

class CrabEnemy : public Engine::Sprite {
public:
    bool hasSpoken = false;
    std::string targetSceneName;
    float animationTimer = 0.0f;
    float animationInterval = 0.3f;
    int animationFrame = 0;
    std::string baseImagePath;

    CrabEnemy(float x, float y, std::string baseImagePath, std::string targetSceneName);

    void Update(float deltaTime) override;
    void OnTouch(); // Just the declaration
};
#endif //CRABENEMY_HPP
