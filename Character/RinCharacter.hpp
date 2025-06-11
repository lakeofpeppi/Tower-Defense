// //
// // Created by Angelie Melisa on 2025/6/9.
// //
//
//
// #ifndef PLAYERCHARACTER_HPP
// #define PLAYERCHARACTER_HPP
// #include <list>
// #include <vector>
// #include <string>
//
// #include "Engine/Point.hpp"
// #include "Engine/Group.hpp"
// #include "Engine/Sprite.hpp"
// #include "UI/Component/Label.hpp"
// #include "Engine/IScene.hpp"
// #include "Engine/GameEngine.hpp"
//
// class Bullet;
// class PlayScene;
//
// enum Enum_Direction{
//     DIRECTION_LEFT, // 0
//     DIRECTION_RIGHT, // 1
//     DIRECTION_FRONT, // 2
//     DIRECTION_BACK, // 3
//     //DIRECTION_UPLEFT, // 4
//     //DIRECTION_UPRIGHT,// 5
//     //DIRECTION_DOWNLEFT,// 6
//     //DIRECTION_DOWNRIGHT, // 7
// };
// enum BlockType {
//     BLANK,
//     BASE_GRASS,
//     HOUSE_BOOK,
//     HOUSE_INVENTORY,
//     NPC_TALKER,
//     ENEMY_1,
//     NPC_JASON,
//     //ENEMY_2,// 10
//     ENEMY_3 // 11
// };
//
//
// enum ItemType{
//     ITEM_BLANK,
//     ITEM_a,
//     ITEM_b,
//     ITEM_c,
// };
//
// class PlayerCharacter : public Engine::Sprite {
// public:
//     float speedMultiplier = 1.0f;
//     int money = 0;
//
//     float x = 0, y = 0;
//     float size = 64;
//     float collission_radius = 32; // think of this as a hitbox
//     float speed = 2.0;
//     float currentHP = 100, maxHP = 100;
//     float attackDamage = 5;
//     int currentEXP = 0, maxEXP = 50;
//     int playerLevel = 1;
//
//     Enum_Direction directionFacing = DIRECTION_DOWN;
//     IObject * charSpriteObj = nullptr;
//
//     int healthPotion = 0;
//     int missile = 0;
//     int shield = 0;
//     bool canMove = true;
// public:
//     PlayerCharacter(float x, float y, float speed, float hp, int money, int blockSize, std::string mapID, PlayerEntry entry);
//     ~PlayerCharacter();
//     void Update(float deltaTime) override;
//     void Draw() const override;
//
//     void InitializeProfile(PlayerEntry entry);
//     std::string currentMapID;
//
//     // * Player Functionalities
//     void UpdateCharacterDirection(float deltaTime);
//     bool CollisionCheck(float x, float y, Enum_Direction dir);
//     void SetMovementState(int keycode, bool keyDown);
//     void OnPlayerDead();
//     void UpdateSprite();
//     void ResetMovementInput();
//     Engine::Point GetPlayerPositionAtMap();
//     void OverlapWithItem(ItemType, int, int);
//     float GetCurrentHP() {return currentHP;}
//     void SetCurrentHP(float newVal, bool shouldClamp = true);
//     float GetPlayerSpeed() {return speed;}
//     void SetPlayerSpeed(float newSpeed) {speed = newSpeed;}
//     float GetMaxHP() {return maxHP;}
//     void SetMaxHP(float newMaxHP) {maxHP = newMaxHP;}
//     int GetMoney() {return money;}
//     void SetMoney(int newAmount) {money = newAmount;}
//     void AddEXP(int amount);
//     int GetCurrentEXP() { return currentEXP;}
//     int GetCurrentLevel() { return playerLevel;}
//
//
//
//
//     void ConstructPlayerHUD();
//     void DrawPlayerHUD() const;
//     void DestroyPlayerHUD();
//
//     void CheckPointSave(std::vector<std::vector<ItemType>> itemData, std::vector<std::vector<BlockType>> blockData);
//     void LoadPlayerEntryData(std::string ); // * To Load from profile lists
//     void SaveSceneItemBlockData(std::vector<std::vector<ItemType>>, std::vector<std::vector<BlockType>>);
//
//     void UpdateItemHotBar(); // * to update UI amount of items
//
//     // ? Interaction
//     bool canInteract = false;
//     int objToInteract_PosY = -1, objToInteract_PosX = -1;
//
//     bool isMoving = false;
//     std::vector<size_t> currentFrame = {0,0,0,0};
//     float frameTime = 0.1f; // Time for each frame in second
//     float timeSinceLastFrame = 0.0f; // Store delta time here
//     std::vector<std::vector<std::string>> walkSprites = {
//         { "walk_left_1.png", "char_walk_left_2.png", "char_walk_left_3.png" },
//         { "char_walk_right_1.png", "char_walk_right_2.png", "char_walk_right_3.png" },
//         { "char_walk_up_1.png", "char_walk_up_2.png", "char_walk_up_3.png" },
//         { "char_walk_down_1.png", "char_walk_down_2.png", "char_walk_down_3.png" }
//     };
//
//     std::vector<std::string> idleSprites = {"char_idle_down.png", "char_idle_up.png", "char_idle_left.png", "char_idle_right.png"};
//
//     mutable float chestBGdelay = 1.5f;
// };
//
// #endif

#pragma once
#include "Engine/Sprite.hpp"
#include "Scene/BasePlayScene.hpp"
class PlayScene;


class RinCharacter : public Engine::Sprite {
public:
    RinCharacter(float x, float y);
    void Update(float deltaTime) override;
    void SetPlayScene(BasePlayScene* playScene);
    int animationFrame = 0; // Current frame index (0, 1, 2)
    float animationTimer = 0; // Timer to change frame
    const float animationInterval = 0.1f; // Change frame every 0.1 second
    enum Direction { DOWN, LEFT, RIGHT, UP } direction;

private:
    BasePlayScene* playScene;
    float speed; // pixels per second
};
