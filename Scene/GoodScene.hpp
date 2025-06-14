//
// Created by Shenice Mau on 6/13/2025.
//

#ifndef GOODSCENE_HPP
#define GOODSCENE_HPP

//
// Created by Shenice Mau on 6/13/2025.
//


#include <vector>
#include <string>
#include "UI/Component/Label.hpp"




#include <memory>

#include "Engine/IScene.hpp"
#include <allegro5/allegro_audio.h>

class GoodScene final : public Engine::IScene {
private:

    std::vector<std::string> dialogueLines;
    Engine::Image* dialogueBoxImage = nullptr;

    Engine::Image* GoodScreen = nullptr;

    int currentLine = 0;
    Engine::Label* dialogueLabel = nullptr;
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;

public:
    explicit GoodScene() = default;
    void Initialize() override;
    void Terminate() override;
    void BackOnClick();
    void NextOnClick(int stage);
    void BGMSlideOnValueChanged(float value);
    void SFXSlideOnValueChanged(float value);
};


#endif //GOODSCENE_HPP