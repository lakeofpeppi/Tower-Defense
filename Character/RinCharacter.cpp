//
// Created by Angelie Melisa on 2025/6/9.
//
#include "RinCharacter.hpp"
#include "Engine/GameEngine.hpp"
#include "Scene/PlayScene.hpp"
#include "Engine/Resources.hpp"
#include "Engine/Sprite.hpp"
#include<iostream>
#include <allegro5/allegro.h>
//#include "PlayScene.hpp"

#include <cmath>

RinCharacter::RinCharacter(float x, float y)
    : Engine::Sprite("Character/down_1.png", x, y, PlayScene::BlockSize, PlayScene::BlockSize, 0.5, 0.5), playScene(nullptr), speed(200), direction(DOWN) {}

void RinCharacter::SetPlayScene(PlayScene* playScene) {
    this->playScene = playScene;
}

void RinCharacter::Update(float deltaTime) {
    //std::cout << "[Rin] Update called\n";
    std::cout << "Rin Position: (" << Position.x << ", " << Position.y << ")\n";

    if (!playScene) return;

    float newX = Position.x;
    float newY = Position.y;
    bool moving = false;

    if (playScene->keyUpDown) {
        newY -= speed * deltaTime;
        direction = UP;
        moving = true;
    }
    if (playScene->keyDownDown) {
        newY += speed * deltaTime;
        direction = DOWN;
        moving = true;
    }
    if (playScene->keyLeftDown) {
        newX -= speed * deltaTime;
        direction = LEFT;
        moving = true;
    }
    if (playScene->keyRightDown) {
        newX += speed * deltaTime;
        direction = RIGHT;
        moving = true;
    }
    float oldX = Position.x, oldY = Position.y;
    Position.x = newX;
    Position.y = newY;

    int gridX = std::floor(newX / PlayScene::BlockSize);
    int gridY = std::floor(newY / PlayScene::BlockSize);

    //if (gridX < 0 || gridX >= PlayScene::MapWidth || gridY < 0 || gridY >= PlayScene::MapHeight)
     //   return;
/*
    if (playScene->mapState[gridY][gridX] == PlayScene::TILE_GRASS) {
        Position.x = gridX * PlayScene::BlockSize + PlayScene::BlockSize / 2;
        Position.y = gridY * PlayScene::BlockSize + PlayScene::BlockSize / 2;
    }
    */
    if (playScene->mapState[gridY][gridX] != PlayScene::TILE_GRASS) {
        Position.x = oldX;
        Position.y = oldY;
    }
    if (moving) {
        animationTimer += deltaTime;
        if (animationTimer >= animationInterval) {
            animationTimer = 0;
            animationFrame = (animationFrame + 1) % 3; // Loop 0-2
        }
    } else {
        animationFrame = 1; // Stay on middle frame when idle
    }

    // Set the correct sprite frame
    std::string dirStr;
    switch (direction) {
        case DOWN: dirStr = "down"; break;
        case LEFT: dirStr = "left"; break;
        case RIGHT: dirStr = "right"; break;
        case UP: dirStr = "up"; break;
    }
    std::string filename = "Character/" + dirStr + "_" + std::to_string(animationFrame + 1) + ".png";
    SetBitmap(Engine::Resources::GetInstance().GetBitmap(filename));
    //std::cout << "speed=" << speed << ", dt=" << deltaTime << "\n";

}
