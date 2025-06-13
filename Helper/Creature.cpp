//
// Created by Angelie Melisa on 2025/6/13.
//

#include "Engine/Resources.hpp"
#include "Engine/GameEngine.hpp"
#include "Helper/Creature.hpp"
Creature::Creature(float x, float y, std::string baseImagePath, int frameCount, float animationInterval, float w, float h)
    : Engine::Sprite(baseImagePath + "_1.png", x, y, w, h, 0.5, 0.5),
      baseImagePath(baseImagePath), frameCount(frameCount), animationInterval(animationInterval) {}



void Creature::Update(float deltaTime) {
    animationTimer += deltaTime;
    if (animationTimer >= animationInterval) {
        animationTimer = 0.0f;
        animationFrame = (animationFrame + 1) % frameCount;
        std::string filename = baseImagePath + "_" + std::to_string(animationFrame + 1) + ".png";
        SetBitmap(Engine::Resources::GetInstance().GetBitmap(filename));
    }
}