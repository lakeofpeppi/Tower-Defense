#include <vector>
#include <string>
#include "UI/Component/Label.hpp"
#ifndef INTROSCENE_HPP
#define INTROSCENE_HPP



#include <memory>

#include "Engine/IScene.hpp"
#include <allegro5/allegro_audio.h>

class IntroScene final : public Engine::IScene {
private:

    std::vector<std::string> dialogueLines;
    std::shared_ptr<Engine::Image> dialogueBoxImage;
    std::shared_ptr<Engine::Image> rin_normal;
    std::shared_ptr<Engine::Image> rin_worry;
    std::shared_ptr<Engine::Image> rin_close;
    std::shared_ptr<Engine::Image> blackScreen;

    int currentLine = 0;
    Engine::Label* dialogueLabel = nullptr;
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;

public:
    explicit IntroScene() = default;
    void Initialize() override;
    void Terminate() override;
    void BackOnClick(int stage);
    void NextOnClick(int stage);
    void BGMSlideOnValueChanged(float value);
    void SFXSlideOnValueChanged(float value);
};

#endif //INTROSCENE_HPP