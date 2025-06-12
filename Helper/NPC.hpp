//
// Created by Angelie Melisa on 2025/6/11.
//

#ifndef NPCTALKER_HPP
#define NPCTALKER_HPP
//
// Created by Angelie Melisa on 2025/6/11.
//
#pragma once
#include "Engine/Sprite.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Resources.hpp"

class NPC : public Engine::Sprite {
public:
    std::string targetSceneName;
    float animationTimer = 0.0f;
    float animationInterval = 1.0f; // 5 fps, adjust to taste
    int animationFrame = 0; // 0-2 for 3 frames
    std::string baseImagePath;

    NPC(float x, float y, std::string baseImagePath, std::string targetSceneName)
        : Engine::Sprite(baseImagePath + "_1.png", x, y, 128, 128, 0.5, 0.5),
          targetSceneName(targetSceneName), baseImagePath(baseImagePath) {}

    void Update(float deltaTime) override {
        animationTimer += deltaTime;
        if (animationTimer >= animationInterval) {
            animationTimer = 0.0f;
            animationFrame = (animationFrame + 1) % 4; // Loop 0-3 for 4 frames
            std::string filename = baseImagePath + "_" + std::to_string(animationFrame + 1) + ".png";
            SetBitmap(Engine::Resources::GetInstance().GetBitmap(filename));
        }
    }


    void OnTouch() {
        Engine::GameEngine::GetInstance().ChangeScene(targetSceneName);
    }
};

#endif //NPCTALKER_HPP
