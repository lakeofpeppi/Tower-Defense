//
// Created by Angelie Melisa on 2025/5/26.
//

#ifndef FLASHEFFECT_HPP
#define FLASHEFFECT_HPP


#include "Engine/IObject.hpp"
#include "allegro5/allegro_color.h"

class FlashEffect : public Engine::IObject {
protected:
    float timeTicks;
    float duration;
public:
    FlashEffect(float duration = 1.0f); // 0.5 seconds default
    void Update(float deltaTime) override;
    void Draw() const override;
};

#endif //FLASHEFFECT_HPP
