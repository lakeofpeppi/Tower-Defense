#pragma once

#include <functional>
#include <memory>
#include <string>
#include "UI/Component/Label.hpp"
#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Point.hpp"
#include "Engine/Resources.hpp"
#include "UI/Component/ImageButton.hpp"
#include "Scene/PlayScene.hpp"

class MemoryScene : public Engine::IScene {
public:
    void Initialize() override;
    void NextOnClick(int stage);
    void Memory1OnClick();
    void Memory2OnClick();
    void GiveUpMemory1();
    void GiveUpMemory2();
    void BackToMemorySelection();
    void Terminate();

private:
    void ShowMemory(int memoryIndex, std::string textLine1, std::string textLine2, std::function<void()> giveUpCallback);



    Engine::Image* memory1Photo = nullptr;
    Engine::Image* memory2Photo = nullptr;
    Engine::Label* memory1Dialogue = nullptr;
    Engine::Label* memory2Dialogue = nullptr;
    Engine::Label* dialogueLabel = nullptr;
    Engine::ImageButton* memory1Button = nullptr;
    Engine::ImageButton* memory2Button = nullptr;
    Engine::Label* labelMemory1 = nullptr;
    Engine::Label* labelMemory2 = nullptr;
    Engine::ImageButton* giveUpButton = nullptr;
    Engine::ImageButton* backButton = nullptr;
    Engine::Label* labelGiveUp = nullptr;
    Engine::Label* labelBack = nullptr;
    Engine::ImageButton* nextButton;
    Engine::Label* labelNext;
    Engine::Label* dialogue1 = nullptr;
    Engine::Label* dialogue2 = nullptr;
    Engine::Image* memoryPhoto1 = nullptr;
    Engine::Image* memoryPhoto2 = nullptr;
    Engine::ImageButton* memoryPhotoButton;
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;




    int screenW = 0, screenH = 0;
    std::vector<std::string> dialogueLines;
    std::vector<Engine::Label*> dialogueLabels;

    int currentLine = 0;
    enum class MemoryState { INTRO, CHOOSE_MEMORY } currentState;
    Engine::Image* blackScreen = nullptr;
    Engine::Image* dialogueBoxImage = nullptr;
};
