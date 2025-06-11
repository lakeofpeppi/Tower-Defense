//
// Created by Angelie Melisa on 2025/6/11.
//

#ifndef HOUSE_HPP
#define HOUSE_HPP

#pragma once
#include "Engine/Sprite.hpp"

class House : public Engine::Sprite {
public:
    std::string targetSceneName;

    House(float x, float y, std::string img, std::string targetSceneName)
        : Engine::Sprite(img, x, y, 128, 128, 0.5, 0.5), targetSceneName(targetSceneName) {}

    void OnTouch() {
        Engine::GameEngine::GetInstance().ChangeScene(targetSceneName);
    }
};

#endif //HOUSE_HPP