//
// Created by Angelie Melisa on 2025/6/12.
//

#ifndef FENCE_HPP
#define FENCE_HPP

#pragma once
#include "Engine/Sprite.hpp"

// A non-traversable obstacle: on touch, it blocks the player rather than changing scenes
class Fence : public Engine::Sprite {
public:
    // Constructor: draws the fence sprite at 256×256 pixels on-screen (anchor at center)
    Fence(float x, float y, const std::string &img)
        : Engine::Sprite(
            img,      // image file path
            x, y,     // position
            64, 64, // draw width & height
            0.5f, 0.5f // anchor (center)
        )
    {}

    // Override OnTouch to do nothing (blocking behavior is handled elsewhere in collision logic)
    void OnTouch()  {
        // No scene change; collision with a fence simply prevents movement
    }
};

#endif // FENCE_HPP

