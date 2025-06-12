//
// Created by Angelie Melisa on 2025/6/13.
//

#ifndef JELLYFISH_HPP
#define JELLYFISH_HPP
#pragma once
#include "Engine/Sprite.hpp"
#include <string>

class JellyFishEnemy : public Engine::Sprite {
public:
    bool hasSpoken = false;
    std::string targetSceneName;
    float animationTimer = 0.0f;
    float animationInterval = 0.2f;
    int animationFrame = 0;
    std::string baseImagePath;

    JellyFishEnemy(float x, float y, std::string baseImagePath, std::string targetSceneName);

    void Update(float deltaTime) override;
    void OnTouch(); // Just the declaration
};
#endif //JELLYFISH_HPP
