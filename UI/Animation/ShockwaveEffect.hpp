//
// Created by Angelie Melisa on 2025/5/26.
//

#ifndef SHOCKWAVEEFFECT_HPP
#define SHOCKWAVEEFFECT_HPP
#include "Engine/IObject.hpp"
#include <allegro5/allegro_color.h>

class ShockwaveEffect : public Engine::IObject {
public:
    ALLEGRO_COLOR color;
    float timeTicks, duration;
    float startRadius, endRadius;
    ShockwaveEffect(float x, float y,
                        float startRadius = 50.0f,
                        float endRadius = 600.0f,
                        float duration = 0.4f,
                        ALLEGRO_COLOR color = al_map_rgba(255, 255, 255, 255));
    void Update(float deltaTime) override;
    void Draw() const override;
};

#endif //SHOCKWAVEEFFECT_HPP
