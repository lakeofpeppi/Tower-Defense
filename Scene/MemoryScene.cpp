#include <allegro5/allegro_audio.h>
#include <functional>
#include <memory>
#include <string>

#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Point.hpp"
#include "Engine/Resources.hpp"
#include "PlayScene.hpp"
#include "Scene/SettingsScene.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "UI/Component/Slider.hpp"
#include "Scene/SettingsScene.hpp"
#include "IntroScene.hpp"
#include "PrincessScene.hpp"
#include "Scene/MemoryScene.hpp"
#include "Scene/BadScene.hpp"
#include <iostream>
#include "GameData.hpp"

void MemoryScene::Initialize() {
    screenW = Engine::GameEngine::GetInstance().GetScreenSize().x;
    screenH = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = screenW / 2;
    int halfH = screenH / 2;

    // Add a black screen at the start
    blackScreen = new Engine::Image("play/black.png", 0, 0, screenW, screenH);
    blackScreen->Visible = true;
    AddNewObject(blackScreen);

    dialogueLines = {
        "You have obtained the glass rose.",
        "In order to go to the next destination, you must give up a memory."
    };
    currentState = MemoryState::INTRO;

    // Dialogue box setup
    dialogueBoxImage = new Engine::Image("play/dialogue.png", halfW - 600, screenH - 210, 1250, 150);
    dialogueBoxImage->Visible = false;
    AddNewObject(dialogueBoxImage);

    for (size_t i = 0; i < dialogueLines.size(); ++i) {
        Engine::Label* label = new Engine::Label(dialogueLines[i], "To The Point.ttf", 70, screenW / 2, screenH / 2, 255, 255, 255, 255, 0.5, 0.5);
        label->Visible = (i == 0); // Show only first line
        dialogueLabels.push_back(label);
        AddNewObject(label);
    }


    // Memory Buttons (Initially hidden)
    int buttonWidth = 200;
    int buttonHeight = 80;
    int spacing = 40;
    int centerX = 896 - buttonWidth / 2;
    // Next button
    nextButton = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW + 680, halfH + 500, 200, 80);
    nextButton->SetOnClickCallback(std::bind(&MemoryScene::NextOnClick, this, 1));
    AddNewControlObject(nextButton);

    // Next button label
    labelNext = new Engine::Label("next", "To The Point.ttf", 80,
                                  halfW + 680 + 100, // X center of button
                                  halfH + 500 + 40,  // Y center of button
                                  255, 255, 255, 255, 0.5, 0.5);
    AddNewObject(labelNext);




    memory1Button = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", centerX - 50, halfH - 100, buttonWidth + 100, buttonHeight);
    memory1Button->SetOnClickCallback(std::bind(&MemoryScene::Memory1OnClick, this));
    memory1Button->Visible = false;
    AddNewControlObject(memory1Button);

    labelMemory1 = new Engine::Label("Memory 1", "pirulen.ttf", 32, centerX + buttonWidth / 2, halfH - 100 + buttonHeight / 2, 255, 255, 255, 255, 0.5, 0.5);
    labelMemory1->Visible = false;
    AddNewObject(labelMemory1);

    memory2Button = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", centerX - 50, halfH + spacing + buttonHeight, buttonWidth + 100, buttonHeight);
    memory2Button->SetOnClickCallback(std::bind(&MemoryScene::Memory2OnClick, this));
    memory2Button->Visible = false;
    AddNewControlObject(memory2Button);

    labelMemory2 = new Engine::Label("Memory 2", "pirulen.ttf", 32, centerX + buttonWidth / 2, halfH + spacing + buttonHeight + buttonHeight / 2, 255, 255, 255, 255, 0.5, 0.5);
    labelMemory2->Visible = false;
    AddNewObject(labelMemory2);

    memoryPhoto1 = new Engine::Image("play/rin strength.png", 300, 150, 1200, 800);
    memoryPhoto1->Visible = false;
    AddNewObject(memoryPhoto1);

    memoryPhoto2 = new Engine::Image("play/rin speed.png", 300, 150, 1200, 800);
    memoryPhoto2->Visible = false;
    AddNewObject(memoryPhoto2);


    dialogue1 = new Engine::Label("", "To The Point.ttf", 60, screenW / 2, 980, 255, 255, 255, 255, 0.5, 0.5);
    dialogue1->Visible = false;
    AddNewObject(dialogue1);

    dialogue2 = new Engine::Label("", "To The Point.ttf", 60, screenW / 2, 1040, 255, 255, 255, 255, 0.5, 0.5);
    dialogue2->Visible = false;
    AddNewObject(dialogue2);

    int buttonY = screenH - 150;

    int totalWidth = buttonWidth * 2 + spacing;
    int giveUpX = halfW - totalWidth / 2;
    int backX = giveUpX + buttonWidth + spacing;

    giveUpButton = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", giveUpX, buttonY, buttonWidth, buttonHeight);
    giveUpButton->Visible = false;
    AddNewControlObject(giveUpButton);

    labelGiveUp = new Engine::Label("Give Up", "pirulen.ttf", 32, giveUpX + buttonWidth / 2, buttonY + buttonHeight / 2, 255, 255, 255, 255, 0.5, 0.5);
    labelGiveUp->Visible = false;
    AddNewObject(labelGiveUp);

    backButton = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", backX, buttonY, buttonWidth, buttonHeight);
    backButton->SetOnClickCallback(std::bind(&MemoryScene::BackToMemorySelection, this));
    backButton->Visible = false;
    AddNewControlObject(backButton);

    labelBack = new Engine::Label("Back", "pirulen.ttf", 32, backX + buttonWidth / 2, buttonY + buttonHeight / 2, 255, 255, 255, 255, 0.5, 0.5);
    labelBack->Visible = false;
    AddNewObject(labelBack);
    bgmInstance = AudioHelper::PlaySample("book.mp3", true, AudioHelper::BGMVolume);
}

void MemoryScene::NextOnClick(int stage) {
    AudioHelper::PlaySample("press.mp3");
    if (currentState == MemoryState::INTRO) {
        dialogueLabels[currentLine]->Visible = false;
        currentLine++;

        if (currentLine < dialogueLabels.size()) {
            dialogueLabels[currentLine]->Visible = true;
        } else {
            // Transition to memory selection
            currentState = MemoryState::CHOOSE_MEMORY;
            blackScreen->Visible = false;

            for (auto* label : dialogueLabels)
                label->Visible = false;

            nextButton->Visible = false;
            labelNext->Visible = false;

            memory1Button->Visible = true;
            labelMemory1->Visible = true;
            memory2Button->Visible = true;
            labelMemory2->Visible = true;
        }
    }
}

void MemoryScene::ShowMemory(int memoryIndex, std::string textLine1, std::string textLine2, std::function<void()> giveUpCallback) {
    // Hide memory selection UI
    memory1Button->Visible = false;
    memory2Button->Visible = false;
    labelMemory1->Visible = false;
    labelMemory2->Visible = false;

    // Hide all memory photos first
    memoryPhoto1->Visible = false;
    memoryPhoto2->Visible = false;

    // Show the selected photo
    if (memoryIndex == 1) memoryPhoto1->Visible = true;
    if (memoryIndex == 2) memoryPhoto2->Visible = true;

    // Update dialogue
    dialogue1->Visible = true;
    dialogue1->Text = textLine1;

    dialogue2->Visible = true;
    dialogue2->Text = textLine2;

    giveUpButton->Visible = true;
    labelGiveUp->Visible = true;
    backButton->Visible = true;
    labelBack->Visible = true;

    giveUpButton->SetOnClickCallback(giveUpCallback);
}



void MemoryScene::Memory1OnClick() {
    AudioHelper::PlaySample("press.mp3");
    ShowMemory(
        1,
        "Stranded in a forest, Rin must bring his injured sister home safely.",
        "By giving up this memory, you are giving up strength.",
        std::bind(&MemoryScene::GiveUpMemory1, this));
}
void MemoryScene::Terminate() {
    if (bgmInstance) {
        AudioHelper::StopSample(bgmInstance);
        bgmInstance.reset();  // safer and clearer
    }
    IScene::Terminate();
}
void MemoryScene::Memory2OnClick() {
    AudioHelper::PlaySample("press.mp3");
    ShowMemory(
        2,
        "In the middle of the night, Rin's house was robbed and destroyed.",
        "By giving up this memory, you are giving up speed.",
        std::bind(&MemoryScene::GiveUpMemory2, this));
}

void MemoryScene::GiveUpMemory1() {
    AudioHelper::PlaySample("press.mp3");
    GameData::strength -= 20;
    std::cout << "Giving up Memory 1 (Strength)" << std::endl;
    Engine::GameEngine::GetInstance().ChangeScene("ocean");
}

void MemoryScene::GiveUpMemory2() {
    AudioHelper::PlaySample("press.mp3");
    GameData::speed -= 20;
    std::cout << "Giving up Memory 2 (Speed)" << std::endl;
    Engine::GameEngine::GetInstance().ChangeScene("ocean");
}

void MemoryScene::BackToMemorySelection() {
    AudioHelper::PlaySample("press.mp3");
    currentState = MemoryState::CHOOSE_MEMORY;


    dialogue1->Visible = false;
    dialogue2->Visible = false;
    memoryPhoto1->Visible = false;
    memoryPhoto2->Visible = false;

    giveUpButton->Visible = false;
    labelGiveUp->Visible = false;
    backButton->Visible = false;
    labelBack->Visible = false;

    memory1Button->Visible = true;
    labelMemory1->Visible = true;
    memory2Button->Visible = true;
    labelMemory2->Visible = true;
}
