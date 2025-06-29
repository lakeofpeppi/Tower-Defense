//
// Created by Angelie Melisa on 2025/6/13.
//
//
// Created by Angelie Melisa on 2025/6/12.
//

#include "Enemy/JellyFishEnemy.hpp"
#include "Engine/Resources.hpp"
#include "Engine/GameEngine.hpp"
#include "Scene/VillageScene.hpp" // FULL include allowed here
#include <iostream>

JellyFishEnemy::JellyFishEnemy(float x, float y, std::string baseImagePath, std::string targetSceneName)
    : Engine::Sprite(baseImagePath + "_1.png", x, y, 128, 128, 0.5, 0.5),
      targetSceneName(targetSceneName), baseImagePath(baseImagePath) {}

void JellyFishEnemy::Update(float deltaTime) {
    animationTimer += deltaTime;
    if (animationTimer >= animationInterval) {
        animationTimer = 0.0f;
        animationFrame = (animationFrame + 1) % 17;
        std::string filename = baseImagePath + "_" + std::to_string(animationFrame + 1) + ".png";
        SetBitmap(Engine::Resources::GetInstance().GetBitmap(filename));
    }
}

void JellyFishEnemy::OnTouch() {
    if (hasSpoken) return;
    hasSpoken = true;
    std::cout << "[DEBUG] OrcEnemy touched. Changing scene to 'sea'." << std::endl;
    Engine::GameEngine::GetInstance().ChangeScene("sea");
    //if (hasSpoken) return;
    /*
    VillageScene* villageScene = dynamic_cast<VillageScene*>(Engine::GameEngine::GetInstance().GetScene("village"));
    if (villageScene) {
        villageScene->ShowDialogue({
            "...", //0 -> toma shock
        });
        hasSpoken = true;
        */
    }

