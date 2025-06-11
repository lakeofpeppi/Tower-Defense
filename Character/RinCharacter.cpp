//
// Created by Angelie Melisa on 2025/6/9.
//
#include "RinCharacter.hpp"
#include "Engine/GameEngine.hpp"
#include "Scene/BasePlayScene.hpp"
#include "Scene/PlayScene.hpp"
#include "Scene/OceanScene.hpp"
#include "Engine/Resources.hpp"
#include "Engine/Sprite.hpp"
#include<iostream>
#include <allegro5/allegro.h>
//#include "PlayScene.hpp"

#include <cmath>

RinCharacter::RinCharacter(float x, float y)
    : Engine::Sprite("Character/down_1.png", x, y, PlayScene::BlockSize, PlayScene::BlockSize, 0.5, 0.5), playScene(nullptr), speed(200), direction(DOWN) {}
//PlayScene::BlockSize, PlayScene::BlockSize --> original w and h
void RinCharacter::SetPlayScene(BasePlayScene* playScene) {
    this->playScene = playScene;
}

void RinCharacter::Update(float deltaTime) {
    std::cout << "Rin Position: (" << Position.x << ", " << Position.y << ")\n";
    if (!playScene) return;

    // Cast to both possible scene types
    auto* ps = dynamic_cast<PlayScene*>(playScene);
    //auto* os = dynamic_cast<OceanScene*>(playScene);

    // Unified key states
    bool keyUp = false, keyDown = false, keyLeft = false, keyRight = false;
    if (ps) {
        keyUp = ps->keyUpDown;
        keyDown = ps->keyDownDown;
        keyLeft = ps->keyLeftDown;
        keyRight = ps->keyRightDown;
    } /*else if (os) {
        keyUp = os->keyUpDown;
        keyDown = os->keyDownDown;
        keyLeft = os->keyLeftDown;
        keyRight = os->keyRightDown;
    }*/

    float newX = Position.x;
    float newY = Position.y;
    bool moving = false;

    // Movement logic
    if (keyUp) {
        newY -= speed * deltaTime;
        direction = UP;
        moving = true;
    }
    if (keyDown) {
        newY += speed * deltaTime;
        direction = DOWN;
        moving = true;
    }
    if (keyLeft) {
        newX -= speed * deltaTime;
        direction = LEFT;
        moving = true;
    }
    if (keyRight) {
        newX += speed * deltaTime;
        direction = RIGHT;
        moving = true;
    }

    float oldX = Position.x, oldY = Position.y;
    Position.x = newX;
    Position.y = newY;

    int gridX = std::floor(newX / playScene->BlockSize);
    int gridY = std::floor(newY / playScene->BlockSize);

    // Safe check for mapState bounds
    if (playScene &&
        gridY >= 0 && gridY < playScene->mapState.size() &&
        gridX >= 0 && gridX < playScene->mapState[gridY].size()) {
        // If the tile is not walkable, revert to old position
        if (!playScene->IsTileWalkable(playScene->mapState[gridY][gridX])) {
            Position.x = oldX;
            Position.y = oldY;
        }
    }


    // Animation logic
    //if (gridX < 0 || gridX >= PlayScene::MapWidth || gridY < 0 || gridY >= PlayScene::MapHeight)
     //   return;
/*
    if (playScene->mapState[gridY][gridX] == PlayScene::TILE_GRASS) {
        Position.x = gridX * PlayScene::BlockSize + PlayScene::BlockSize / 2;
        Position.y = gridY * PlayScene::BlockSize + PlayScene::BlockSize / 2;
    }
    */
    // if (playScene->mapState[gridY][gridX] != PlayScene::TILE_GRASS) {
    //     Position.x = oldX;
    //     Position.y = oldY;
    // }

    if (moving) {
        animationTimer += deltaTime;
        if (animationTimer >= animationInterval) {
            animationTimer = 0;
            animationFrame = (animationFrame + 1) % 3; // Loop 0-2
        }
    } else {
        animationFrame = 2; // Stay on middle frame when idle
    }

    // Set the correct sprite frame
    std::string dirStr;
    switch (direction) {
        case DOWN: dirStr = "down"; break;
        case LEFT: dirStr = "left"; break;
        case RIGHT: dirStr = "right"; break;
        case UP: dirStr = "up"; break;
    }
    if (gridX < 0 || gridX >= PlayScene::MapWidth ||
    gridY < 0 || gridY >= PlayScene::MapHeight) {
        std::cerr << "[Warning] Rin attempted to move out of bounds: (" << gridX << ", " << gridY << ")\n";
    }

    std::string filename = "Character/" + dirStr + "_" + std::to_string(animationFrame + 1) + ".png";
    SetBitmap(Engine::Resources::GetInstance().GetBitmap(filename));
    //std::cout << "speed=" << speed << ", dt=" << deltaTime << "\n";

}

