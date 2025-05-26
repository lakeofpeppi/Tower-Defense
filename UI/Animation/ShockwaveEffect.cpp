//
// Created by Angelie Melisa on 2025/5/26.
//

#include "ShockwaveEffect.hpp"
#include <allegro5/allegro_primitives.h>
#include "Engine/GameEngine.hpp"
#include "Engine/IScene.hpp"
#include "Engine/Group.hpp"
#include "Scene/PlayScene.hpp"

ShockwaveEffect::ShockwaveEffect(float x, float y, float startRadius, float endRadius,
                                 float duration, ALLEGRO_COLOR color)
    : IObject(x, y), timeTicks(0), duration(duration), startRadius(startRadius), endRadius(endRadius), color(color)
{}


void ShockwaveEffect::Draw() const {
    float radius = startRadius + (endRadius - startRadius) * (timeTicks / duration);
    al_draw_circle(Position.x, Position.y, radius, color, 3);
}

void ShockwaveEffect::Update(float deltaTime) {
    timeTicks += deltaTime;
    if (timeTicks >= duration) {
        // auto-remove when time is up
        auto* scene = dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
        if (scene) scene->EffectGroup->RemoveObject(objectIterator);
    }
}
