//
// Created by Angelie Melisa on 2025/6/14.
//

#ifndef BATTLESCENE_HPP
#define BATTLESCENE_HPP
#include <allegro5/allegro_audio.h>
#include <memory>

#include "Engine/IScene.hpp"

class BattleScene: public Engine::IScene {
private:
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;

public:
    explicit StageSelectScene() = default;
    void Initialize() override;
    void Terminate() override;
    void PlayOnClick(int stage);
    void ScoreboardOnClick();
    void Ending();
    void BackOnClick(int stage);

    void BGMSlideOnValueChanged(float value);
    void SFXSlideOnValueChanged(float value);
};

#endif //BATTLESCENE_HPP
