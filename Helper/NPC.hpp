// Don't include VillageScene.hpp
#ifndef NPCTALKER_HPP
#define NPCTALKER_HPP

#pragma once
#include "Engine/Sprite.hpp"
#include <string>

class NPC : public Engine::Sprite {
public:
    bool hasSpoken = false;
    std::string targetSceneName;
    float animationTimer = 0.0f;
    float animationInterval = 1.0f;
    int animationFrame = 0;
    std::string baseImagePath;

    NPC(float x, float y, std::string baseImagePath, std::string targetSceneName);

    void Update(float deltaTime) override;
    void OnTouch(); // Just the declaration
};

#endif // NPCTALKER_HPP
