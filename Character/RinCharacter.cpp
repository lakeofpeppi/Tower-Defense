//
// Created by Angelie Melisa on 2025/6/9.
//
#include "RinCharacter.hpp"
#include "Engine/GameEngine.hpp"
#include "Scene/PlayScene.hpp"
#include<iostream>
#include <allegro5/allegro.h>
//#include "PlayScene.hpp"

#include <cmath>

RinCharacter::RinCharacter(float x, float y)
    : Engine::Sprite("Character/down_1.png", x, y, PlayScene::BlockSize, PlayScene::BlockSize, 0.5, 0.5), playScene(nullptr), speed(200) {}

void RinCharacter::SetPlayScene(PlayScene* playScene) {
    this->playScene = playScene;
}

void RinCharacter::Update(float deltaTime) {
    std::cout << "[Rin] Update called\n";
    if (!playScene) return;

    float newX = Position.x;
    float newY = Position.y;

    if (playScene->keyUpDown)    newY -= speed * deltaTime;
    if (playScene->keyDownDown)  newY += speed * deltaTime;
    if (playScene->keyLeftDown)  newX -= speed * deltaTime;
    if (playScene->keyRightDown) newX += speed * deltaTime;

    Position.x = newX;
    Position.y = newY;

    int gridX = std::floor(newX / PlayScene::BlockSize);
    int gridY = std::floor(newY / PlayScene::BlockSize);

    if (gridX < 0 || gridX >= PlayScene::MapWidth || gridY < 0 || gridY >= PlayScene::MapHeight)
        return;

    if (playScene->mapState[gridY][gridX] == PlayScene::TILE_GRASS) {
        Position.x = gridX * PlayScene::BlockSize + PlayScene::BlockSize / 2;
        Position.y = gridY * PlayScene::BlockSize + PlayScene::BlockSize / 2;
    }
    std::cout << "speed=" << speed << ", dt=" << deltaTime << "\n";

}
