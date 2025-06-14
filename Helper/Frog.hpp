//
// Created by Angelie Melisa on 2025/6/14.
//

#ifndef FROG_HPP
#define FROG_HPP
//
// Created by Angelie Melisa on 2025/6/13.
//




#include "Engine/Sprite.hpp"
#include <string>

class Frog : public Engine::Sprite {
public:
    /// \param basePath
    /// \param frames
    /// \param interval
    /// \param w,h
    Frog(float x, float y,
              const std::string& basePath,
              int frames,
              float interval,
              float w = 128, float h = 128);

    void Update(float deltaTime) override;

private:
    std::string _basePath;
    int         _frameCount;
    float       _interval;
    float       _timer    = 0;
    int         _curFrame = 0;
};




#endif //FROG_HPP
