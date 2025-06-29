//
// Created by Angelie Melisa on 2025/6/12.
//
//
// Created by Angelie Melisa on 2025/6/11.
//
#include "Enemy/ScorpionEnemy.hpp"
#include "Engine/Resources.hpp"
#include "Engine/GameEngine.hpp"
#include "Scene/VillageScene.hpp" // FULL include allowed here
#include <iostream>
ScorpionEnemy::ScorpionEnemy(float x, float y, std::string baseImagePath, std::string targetSceneName)
    : Engine::Sprite(baseImagePath + "_1.png", x, y, 128, 128, 0.5, 0.5),
      targetSceneName(targetSceneName), baseImagePath(baseImagePath) {}

void ScorpionEnemy::Update(float deltaTime) {
    animationTimer += deltaTime;
    if (animationTimer >= animationInterval) {
        animationTimer = 0.0f;
        animationFrame = (animationFrame + 1) % 3;
        std::string filename = baseImagePath + "_" + std::to_string(animationFrame + 1) + ".png";
        SetBitmap(Engine::Resources::GetInstance().GetBitmap(filename));
    }
}

void ScorpionEnemy::OnTouch() {
    std::cout << "[DEBUG] OrcEnemy touched. Changing scene to 'orc'." << std::endl;
    Engine::GameEngine::GetInstance().ChangeScene("scorpion");
}
