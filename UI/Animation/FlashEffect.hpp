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
    FlashEffect(float duration = 0.2f); // 0.2 seconds default
    void Update(float deltaTime) override;
    void Draw() const override;
};

#endif //FLASHEFFECT_HPP
