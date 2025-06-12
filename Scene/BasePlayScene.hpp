// //
// // Created by Angelie Melisa on 2025/6/11.
// //
//
// #ifndef BASEPLAYSCENE_HPP
// #define BASEPLAYSCENE_HPP
// // BasePlayScene.hpp
// // BasePlayScene.hpp
//
// #include "Engine/IScene.hpp"
// #include "Engine/Group.hpp"
// #include "Engine/Point.hpp"
//
// class BasePlayScene : public Engine::IScene {
// public:
//     static const int BlockSize = 64;
//     Engine::Group* TileMapGroup;
//     Engine::Group* GroundEffectGroup;
//     Engine::Group* DebugIndicatorGroup;
//     Engine::Group* TowerGroup;
//     Engine::Group* EnemyGroup;
//     Engine::Group* BulletGroup;
//     Engine::Group* EffectGroup;
//     Engine::Group* UIGroup;
//     bool keyUpDown = false;
//     bool keyDownDown = false;
//     bool keyLeftDown = false;
//     bool keyRightDown = false;
//
//     int MapId = 1;
//     std::vector<std::vector<int>> mapDistance;
//     std::vector<std::vector<int>> mapState;
//     virtual bool IsTileWalkable(int tileType) const = 0;
//
//     virtual void ReadMap() = 0;
//     virtual void ReadEnemyWave() = 0;
// };
//
// #endif // BASEPLAYSCENE_HPP
//
//
