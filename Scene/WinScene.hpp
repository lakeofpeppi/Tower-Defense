#ifndef WINSCENE_HPP
#define WINSCENE_HPP
#include "Engine/IScene.hpp"
#include "UI/Component/TextBox.hpp"
#include <allegro5/allegro_audio.h>

class WinScene final : public Engine::IScene {
private:
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmId;
    float ticks;
    Engine::TextBox* nameInput;
    int finalScore = 0; // will later be passed from PlayScene


public:
    void SetFinalScore(int score);
    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void BackOnClick(int stage);
    void OnKeyDown(int keyCode) override;

};

#endif   // WINSCENE_HPP
