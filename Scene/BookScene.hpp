//
// Created by Shenice Mau on 6/11/2025.
//

#ifndef BOOKSCENE_HPP
#define BOOKSCENE_HPP


#include <vector>
#include <string>
#include "UI/Component/Label.hpp"



#include <memory>

#include "Engine/IScene.hpp"
#include <allegro5/allegro_audio.h>

class BookScene final : public Engine::IScene {
private:

    std::vector<std::string> dialogueLines;

    Engine::Image* dialogueBoxImage = nullptr;
    Engine::Image* rin_normal = nullptr;
    Engine::Image* rin_worry = nullptr;
    Engine::Image* rin_close = nullptr;
    Engine::Image* blackScreen = nullptr;
    Engine::Image* page1 = nullptr;
    Engine::Image* page2 = nullptr;
    Engine::Image* page3 = nullptr;



    int currentLine = -1;
    Engine::Label* dialogueLabel = nullptr;
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;

public:
    explicit BookScene() = default;
    void Initialize() override;
    void Terminate() override;
    void BackOnClick(int stage);
    void NextOnClick(int stage);
    void BGMSlideOnValueChanged(float value);
    void SFXSlideOnValueChanged(float value);
};

#endif //BOOKSCENE_HPP