#ifndef SOLDIERUNIT_HPP
#define SOLDIERUNIT_HPP

#include "Turret.hpp"

class Enemy;

class SoldierUnit : public Turret {
public:
    explicit SoldierUnit(float x, float y);
    void Update(float deltaTime) override;
    static const int Price;


private:
    float moveSpeed = 100; // movement speed per second.
    float pathTimer = 0; // to time when to move
    Enemy* followTarget = nullptr;
    //Engine::Point targetPos;
    bool IsInsideValidTile(const Engine::Point& pos) const;
    void CreateBullet() override;
    std::vector<Engine::Point> path;
    void UpdatePathToEnemy();
    Engine::Point Velocity;
};

#endif
