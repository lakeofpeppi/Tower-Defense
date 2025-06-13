//
// Created by Shenice Mau on 6/13/2025.
//

#ifndef PRINCESSSCENE_HPP
#define PRINCESSSCENE_HPP


#include <vector>
#include <string>
#include "UI/Component/Label.hpp"




#include <memory>

#include "Engine/IScene.hpp"
#include <allegro5/allegro_audio.h>

class PrincessScene final : public Engine::IScene {
private:

    std::vector<std::string> dialogueLines;
    Engine::Image* dialogueBoxImage = nullptr;
    Engine::Image* sayuki_normal = nullptr;
    Engine::Image* sayuki_scars = nullptr;
    Engine::Image* sayuki_far = nullptr;
    Engine::Image* sayuki_worry = nullptr;
    Engine::Image* rin_normal = nullptr;
    Engine::Image* sayuki_close = nullptr;
    Engine::Image* blackScreen = nullptr;
    Engine::Image* RoomScreen = nullptr;
    Engine::Image* FireScreen = nullptr;
    Engine::Image* RainScreen = nullptr;
    Engine::Image* AbuseScreen = nullptr;
    Engine::Image* PrayScreen = nullptr;
    Engine::Image* WorshipScreen = nullptr;
    Engine::Image* AngryScreen = nullptr;


    int currentLine = 0;
    Engine::Label* dialogueLabel = nullptr;
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;

public:
    explicit PrincessScene() = default;
    void Initialize() override;
    void Terminate() override;
    void BackOnClick(int stage);
    void NextOnClick(int stage);
    void BGMSlideOnValueChanged(float value);
    void SFXSlideOnValueChanged(float value);
};

#endif //PRINCESSSCENE_HPP