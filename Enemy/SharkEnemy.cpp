//
// Created by Angelie Melisa on 2025/6/13.
//
#include "Enemy/SharkEnemy.hpp"
#include "Engine/Resources.hpp"
#include "Engine/GameEngine.hpp"
#include "Scene/VillageScene.hpp" // FULL include allowed here

SharkEnemy::SharkEnemy(float x, float y, std::string baseImagePath, std::string targetSceneName)
    : Engine::Sprite(baseImagePath + "_1.png", x, y, 128, 128, 0.5, 0.5),
      targetSceneName(targetSceneName), baseImagePath(baseImagePath) {}

void SharkEnemy::Update(float deltaTime) {
    animationTimer += deltaTime;
    if (animationTimer >= animationInterval) {
        animationTimer = 0.0f;
        animationFrame = (animationFrame + 1) % 4;
        std::string filename = baseImagePath + "_" + std::to_string(animationFrame + 1) + ".png";
        SetBitmap(Engine::Resources::GetInstance().GetBitmap(filename));
    }
}

void SharkEnemy::OnTouch() {
    if (hasSpoken) return;
    VillageScene* villageScene = dynamic_cast<VillageScene*>(Engine::GameEngine::GetInstance().GetScene("village"));
    if (villageScene) {
        villageScene->ShowDialogue({
            "...", //0 -> toma shock
        });
        hasSpoken = true;
    }
}
