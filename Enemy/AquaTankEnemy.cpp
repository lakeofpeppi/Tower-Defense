//
// Created by Angelie Melisa on 2025/5/26.
//
#include <string>
#include "AquaTankEnemy.hpp"
#include "Scene/PlayScene.hpp"
#include "UI/Animation/ShockwaveEffect.hpp"
AquaTankEnemy::AquaTankEnemy(int x, int y)
    :Enemy("play/enemy-5.png", x, y, 10, 80, 10, 10)
{
}

void AquaTankEnemy::OnExplode() {
    // add blue ripple effect
    getPlayScene()->EffectGroup->AddNewObject(
         new ShockwaveEffect(Position.x, Position.y,
                             50,         // start radius
                             300,        // end radius
                             0.5f,       // duration
                             al_map_rgba(255, 0, 0, 80))); // red

    Enemy::OnExplode();// run base class explode logic
}