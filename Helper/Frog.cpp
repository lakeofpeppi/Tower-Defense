//
// Created by Angelie Melisa on 2025/6/14.
//
#include "Frog.hpp"
#include "Engine/Resources.hpp"
#include "Engine/GameEngine.hpp"
#include "Scene/PlayScene.hpp"   // for removing from EffectGroup

Frog::Frog(float x, float y,
                     const std::string& basePath,
                     int frames,
                     float interval,
                     float w, float h)
  : Engine::Sprite(basePath + "_1.png", x, y, w, h, 0.5f, 0.5f),
    _basePath(basePath),
    _frameCount(frames),
    _interval(interval)
{}

void Frog::Update(float deltaTime) {
    // advance timer
    _timer += deltaTime;
    if (_timer < _interval) return;
    _timer -= _interval;

    // next frame
    _curFrame++;
    if (_curFrame < _frameCount) {
        // still within sequence → swap bitmap
        std::string fn = _basePath + "_" + std::to_string(_curFrame + 1) + ".png";
        SetBitmap(Engine::Resources::GetInstance().GetBitmap(fn));
    }
    else {
        // animation done → remove self
        if (auto* scene = dynamic_cast<PlayScene*>(
              Engine::GameEngine::GetInstance().GetActiveScene()))
        {
            // assumes Sprite has GetObjectIterator()
            scene->EffectGroup->RemoveObject(GetObjectIterator());
        }
    }
}
