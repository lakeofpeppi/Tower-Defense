//
// Created by Angelie Melisa on 2025/5/26.
//
#include <allegro5/base.h>
#include <cmath>
#include <string>

#include "Bullet/FireBullet.hpp"
#include "Engine/AudioHelper.hpp"
#include "Engine/Group.hpp"
#include "Engine/Point.hpp"
#include "ShovelTool.hpp"
#include "Scene/PlayScene.hpp"

const int ShovelTool::Price = 0;
ShovelTool::ShovelTool(float x, float y)
    : Turret("play/BurningBullet.png", "play/shovel.png", x, y, 200, Price, 0.5) {
    // Move center downward, since we the turret head is slightly biased upward.
    Anchor.y += 8.0f / GetBitmapHeight();
}
void ShovelTool::CreateBullet() {}
