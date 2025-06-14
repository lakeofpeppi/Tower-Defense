//
// Created by Angelie Melisa on 2025/6/12.
//

#ifndef ORCENEMY_HPP
#define ORCENEMY_HPP


#pragma once
#include "Engine/Sprite.hpp"
#include <string>

class OrcEnemy : public Engine::Sprite {
public:
    std::string targetSceneName;
    float animationTimer = 0.0f;
    float animationInterval = 1.0f;
    int animationFrame = 0;
    std::string baseImagePath;

    OrcEnemy(float x, float y, std::string baseImagePath, std::string targetSceneName);

    void Update(float deltaTime) override;
    void OnTouch(); // Just the declaration
};

#endif //ORCENEMY_HPP
