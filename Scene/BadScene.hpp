//
// Created by Shenice Mau on 6/13/2025.
//

#ifndef BADSCENE_HPP
#define BADSCENE_HPP

//
// Created by Shenice Mau on 6/13/2025.
//



//
// Created by Shenice Mau on 6/13/2025.
//


#include <vector>
#include <string>
#include "UI/Component/Label.hpp"




#include <memory>

#include "Engine/IScene.hpp"
#include <allegro5/allegro_audio.h>

class BadScene final : public Engine::IScene {
private:

    std::vector<std::string> dialogueLines;
    Engine::Image* dialogueBoxImage = nullptr;
    Engine::Image* blackScreen = nullptr;
    Engine::Image* BadScreen = nullptr;


    int currentLine = 0;
    Engine::Label* dialogueLabel = nullptr;
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;

public:
    explicit BadScene() = default;
    void Initialize() override;
    void Terminate() override;
    void NextOnClick(int stage);
    void BGMSlideOnValueChanged(float value);
    void SFXSlideOnValueChanged(float value);
};

#endif //BADSCENE_HPP