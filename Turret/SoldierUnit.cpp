#include <allegro5/base.h>
#include <cmath>
#include <string>
#include "SoldierUnit.hpp"
#include "Engine/AudioHelper.hpp"
#include "Engine/Group.hpp"
#include "Engine/Point.hpp"
#include "Enemy/Enemy.hpp"
#include "Scene/PlayScene.hpp"
#include "Bullet/FireBullet.hpp"
#include<queue>
#include<iostream>

const int SoldierUnit::Price = 125;

SoldierUnit::SoldierUnit(float x, float y)
    : Turret("play/floor.png", "play/pink_guy.png", x, y, 180, Price, 0.4) {
    Anchor.y += 8.0f / GetBitmapHeight();
}

bool SoldierUnit::IsInsideValidTile(const Engine::Point& pos) const {
    int x = pos.x / PlayScene::BlockSize;
    int y = pos.y / PlayScene::BlockSize;
    return x >= 0 && x < PlayScene::MapWidth &&
           y >= 0 && y < PlayScene::MapHeight &&
           getPlayScene()->mapState[y][x] == PlayScene::TILE_FLOOR;
}

void SoldierUnit::Update(float deltaTime) {
    Turret::Update(deltaTime);  // Targeting & shooting logic

    if (!followTarget || !followTarget->Visible) {
        followTarget = nullptr;
        for (auto& obj : getPlayScene()->EnemyGroup->GetObjects()) {
            Enemy* e = dynamic_cast<Enemy*>(obj);
            if (e && e->Visible) {
                followTarget = e;
                break;
            }
        }
    }
    pathTimer += deltaTime;
    if (followTarget && pathTimer > 0.5f) {
        UpdatePathToEnemy();
        pathTimer = 0;
    }

    float remainSpeed = moveSpeed * deltaTime;
    while (remainSpeed > 0) {
        if (path.empty()) {
            Velocity = Engine::Point(0, 0);
            break;
        }
        Engine::Point target = path.back() * PlayScene::BlockSize + Engine::Point(PlayScene::BlockSize / 2, PlayScene::BlockSize / 2);
        Engine::Point vec = target - Position;
        Engine::Point normalized = vec.Normalize();
        if (remainSpeed > vec.Magnitude()) {
            Position = target;
            path.pop_back();
            remainSpeed -= vec.Magnitude();
        } else {
            Velocity = normalized * remainSpeed / deltaTime;
            Position = Position + Velocity * deltaTime;
            remainSpeed = 0;
        }
    }

    if (Velocity.Magnitude() > 0)
        Rotation = atan2(Velocity.y, Velocity.x) + ALLEGRO_PI / 2;
}



void SoldierUnit::CreateBullet() {
    Engine::Point diff = Engine::Point(cos(Rotation - ALLEGRO_PI / 2), sin(Rotation - ALLEGRO_PI / 2));
    float rotation = atan2(diff.y, diff.x);
    Engine::Point normalized = diff.Normalize();
    Engine::Point bulletPos = Position + normalized * 36;
    getPlayScene()->BulletGroup->AddNewObject(new FireBullet(bulletPos, diff, rotation, this));
    AudioHelper::PlayAudio("gun.wav");
}

void SoldierUnit::UpdatePathToEnemy() {
    if (!followTarget) return;

    PlayScene* scene = getPlayScene();
    Engine::Point startGrid(
        static_cast<int>(Position.x / PlayScene::BlockSize),
        static_cast<int>(Position.y / PlayScene::BlockSize)
    );
    Engine::Point endGrid(
        static_cast<int>(followTarget->Position.x / PlayScene::BlockSize),
        static_cast<int>(followTarget->Position.y / PlayScene::BlockSize)
    );

    // If already standing on the target
    if (startGrid == endGrid) {
        path.clear();
        return;
    }

    const int W = PlayScene::MapWidth;
    const int H = PlayScene::MapHeight;
    std::vector<std::vector<int>> dist(H, std::vector<int>(W, -1));
    std::vector<std::vector<Engine::Point>> prev(H, std::vector<Engine::Point>(W, Engine::Point(-1, -1)));
    std::queue<Engine::Point> q;

    dist[startGrid.y][startGrid.x] = 0;
    q.push(startGrid);

    while (!q.empty()) {
        Engine::Point p = q.front(); q.pop();
        for (auto dir : PlayScene::directions) {
            Engine::Point np = p + dir;
            if (np.x < 0 || np.y < 0 || np.x >= W || np.y >= H) continue;
            if (scene->mapState[np.y][np.x] != PlayScene::TILE_FLOOR) continue;
            if (dist[np.y][np.x] != -1) continue;
            dist[np.y][np.x] = dist[p.y][p.x] + 1;
            prev[np.y][np.x] = p;
            q.push(np);
            if (np == endGrid) break;  // early exit
        }
    }

    if (dist[endGrid.y][endGrid.x] == -1) {
        path.clear();
        return;  // no path found
    }

    path.clear();
    for (Engine::Point at = endGrid; at != startGrid; at = prev[at.y][at.x])
        path.insert(path.begin(), at);

    std::cout << "[PATH FOUND] Steps: " << path.size() << std::endl;
}

