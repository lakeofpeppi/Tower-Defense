//
// Created by Angelie Melisa on 2025/6/13.
//
#include "Helper/Creature.hpp"
#include "Helper/Frog.hpp"
//#include "Scene/PlayScene.cpp"
#include "Enemy/FrogEnemy.hpp"
#include "Scene/GameData.hpp"
#include "Engine/Resources.hpp"
#include "Engine/GameEngine.hpp"
#include "Character/RinCharacter.hpp"
#include <iostream>
#include "Scene/VillageScene.hpp" // FULL include allowed here

FrogEnemy::FrogEnemy(float x, float y, std::string baseImagePath, std::string targetSceneName)
    : Engine::Sprite(baseImagePath + "_1.png", x, y, 128, 128, 0.5, 0.5),
      targetSceneName(targetSceneName), baseImagePath(baseImagePath) {}

void FrogEnemy::Update(float deltaTime) {
    animationTimer += deltaTime;
    if (animationTimer >= animationInterval) {
        animationTimer = 0.0f;
        animationFrame = (animationFrame + 1) % 4;
        std::string filename = baseImagePath + "_" + std::to_string(animationFrame + 1) + ".png";
        SetBitmap(Engine::Resources::GetInstance().GetBitmap(filename));
    }
}

void FrogEnemy::OnTouch() {
    if (GameData::isInvincible ) {
        std::cout << "[DEBUG] FrogEnemy touched. Changing scene to 'sahara'." << std::endl;
        Engine::GameEngine::GetInstance().ChangeScene("sahara");
        return;
    }
    if (!isdisappearing) {
        isdisappearing = true;
        Visible = false;
        if (auto* scene = dynamic_cast<PlayScene*>(
                Engine::GameEngine::GetInstance().GetActiveScene()
            ))
        {
            // create and add the FrogDeath effect at this sprite’s position
            auto* death = new Frog(
                Position.x,         // use the sprite’s Position
                Position.y,
                "enemy/frog_dead",  // will load frog_dead_1.png … frog_dead_4.png
                4,                  // frame count
                0.3f                // seconds per frame
            );
            scene->EffectGroup->AddNewObject(death);
        }
    }
}

