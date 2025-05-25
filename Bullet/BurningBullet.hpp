//
// Created by Angelie Melisa on 2025/5/26.
//

#ifndef BURNINGBULLET_HPP
#define BURNINGBULLET_HPP
#include "Bullet.hpp"

class Enemy;
class Turret;
namespace Engine {
    struct Point;
}   // namespace Engine

class BurningBullet : public Bullet {
public:
    explicit BurningBullet(Engine::Point position, Engine::Point forwardDirection, float rotation, Turret *parent);
    void OnExplode(Enemy *enemy) override;

};

#endif //BURNINGBULLET_HPP
