//
// Created by Angelie Melisa on 2025/5/26.
//
#include "FlashEffect.hpp"
#include <allegro5/allegro_primitives.h>
#include "Engine/GameEngine.hpp"
#include "Engine/IScene.hpp"
#include "Engine/Group.hpp"
#include "Scene/PlayScene.hpp"

FlashEffect::FlashEffect(float duration) : IObject(0, 0), timeTicks(0), duration(duration) {
    // You can extend this for color or intensity if needed
}

void FlashEffect::Update(float deltaTime) {
    timeTicks += deltaTime;
    if (timeTicks >= duration) {
        auto* scene = dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
        if (scene) scene->EffectGroup->RemoveObject(objectIterator);
    }
}

void FlashEffect::Draw() const {
    float alpha = 1.0f - (timeTicks / duration); // fades out
    if (alpha < 0) alpha = 0;
    ALLEGRO_COLOR flashColor = al_map_rgba_f(1.0, 1.0, 1.0, alpha);
    al_draw_filled_rectangle(0, 0, 1280, 720, flashColor); // Adjust for screen resolution
}
