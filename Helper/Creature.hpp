//
// Created by Angelie Melisa on 2025/6/13.
//

#ifndef CREATURE_HPP
#define CREATURE_HPP
#include "Engine/Sprite.hpp"
#include <string>
class Creature : public Engine::Sprite {
public:
    float animationTimer = 0.0f;
    float animationInterval;  // now set via constructor
    int animationFrame = 0;
    int frameCount;
    std::string baseImagePath;

    Creature(float x, float y, std::string baseImagePath, int frameCount, float animationInterval, float w = 128, float h = 128);

    void Update(float deltaTime) override;
};


#endif //CREATURE_HPP
