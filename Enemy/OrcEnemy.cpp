//
// Created by Angelie Melisa on 2025/6/12.
//

#include "Enemy/OrcEnemy.hpp"
#include "Engine/Resources.hpp"
#include "Engine/GameEngine.hpp"
#include "Scene/ForestScene.hpp" // FULL include allowed here
#include <iostream>


OrcEnemy::OrcEnemy(float x, float y, std::string baseImagePath, std::string targetSceneName)
    : Engine::Sprite(baseImagePath + "_1.png", x, y, 128, 128, 0.5, 0.5),
      targetSceneName(targetSceneName), baseImagePath(baseImagePath) {}

void OrcEnemy::Update(float deltaTime) {
    animationTimer += deltaTime;
    if (animationTimer >= animationInterval) {
        animationTimer = 0.0f;
        animationFrame = (animationFrame + 1) % 6;
        std::string filename = baseImagePath + "_" + std::to_string(animationFrame + 1) + ".png";
        SetBitmap(Engine::Resources::GetInstance().GetBitmap(filename));
    }

}

void OrcEnemy::OnTouch() {
    std::cout << "[DEBUG] OrcEnemy touched. Changing scene to 'orc'." << std::endl;
    Engine::GameEngine::GetInstance().ChangeScene("orc");
}
