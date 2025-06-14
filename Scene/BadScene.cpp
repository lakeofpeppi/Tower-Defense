//
// Created by Shenice Mau on 6/13/2025.
//
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
#include "GoodScene.hpp"
#include "BadScene.hpp"

#include <iostream>
#include <ostream>

#include "PlayScene.hpp"

void BadScene::Initialize() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;

    Engine::ImageButton *btn;


    int wpic = 1900;
    int hpic = 1300;
    AddNewObject(new Engine::Image("play/black.png", 0, 0, wpic, hpic));
    //AddNewObject(new Engine::Image("play/rin dialogue expressions.png", 0, 620, 2700, 600));

    // Fill dialogue lines
    dialogueLines = {
        "                      You chose to take her back", //0
        "                    You have completed your mission", //1
        "Blinded by greed, you chose to sacrifice the girl's freedom for money", //2
        "            The villagers rejoice as they meet her again", //3
        "         but the Princess stares blankly as they worship her again.",//4
        "           Despite making your choice, you feel hollow.... ",//5
        "   As you turn your back and  walk away to continue your journey,", // 6
        "              a sudden, sharp sound echoes behind him", //7
        "                the Princess has taken her own life", // 8
        "", //9
        "                              BAD ENDING ", //--->make the position of this one on the very bottom //5
    };

    // Create the first line label
    float lineHeight = 70;
    float startX = halfW - 600;
    float startY = halfH - 100;
    // Black screen
    // Black screen
    // blackScreen = new Engine::Image("play/black.png", 0, 0, w, h);
    // blackScreen->Visible = false;
    // AddNewObject(blackScreen);  // Scene will manage deletion

    // Dialogue box
    // dialogueBoxImage = new Engine::Image("play/dialogue.png", halfW - 600, h - 210, 1250, 150);
    // dialogueBoxImage->Visible = false;
    // AddNewObject(dialogueBoxImage);
    //
    // // Rin normal
    // rin_normal = new Engine::Image("play/rin normal.png", halfW - 950, h - 480, 720, 480);
    // rin_normal->Visible = false;
    // AddNewObject(rin_normal);
    //
    // // Rin worried
    BadScreen = new Engine::Image("play/bad end.png", 0, 0, w, h);
    BadScreen->Visible = false;
    AddNewObject(BadScreen);



    dialogueLabel = new Engine::Label(dialogueLines[0], "To The Point.ttf", 70, halfW - 500, h - 608, 255, 255, 255, 255, 0.0, 0.5);
    AddNewObject(dialogueLabel);

    int btnX = 1792 - 400 - 20; // Right edge - button width - padding (1372)
    int btnY = 1216 - 100 - 20; // Bottom edge - button height - padding

    //next
    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW + 680, halfH + 500, 200, 80);
    btn->SetOnClickCallback(std::bind(&BadScene::NextOnClick, this, 1));
    AddNewControlObject(btn);

    // Center the label on the button
    AddNewObject(new Engine::Label("next", "To The Point.ttf", 80,
                                   btnX + 300, // btnX + width/2
                                   btnY + 50, // btnY + height/2
                                   255, 255, 255, 255, // black color
                                   0.5, 0.5)); // centered alignment

    // // === CHOICE BUTTON (Left, mirrored) ===
    // int mirroredX = 1792 - btnX - 200; // mirror across the center
    // btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", mirroredX, btnY, 200, 80);
    // btn->SetOnClickCallback(std::bind(&GoodScene::BackOnClick, this));
    // AddNewControlObject(btn);
    // AddNewObject(new Engine::Label("choice", "To The Point.ttf", 80,
    //     mirroredX + 100,  // x + width / 2
    //     btnY + 40,        // y + height / 2
    //     255, 255, 255, 255, 0.5, 0.5));



    bgmInstance = AudioHelper::PlaySample("princess-meet.mp3", true, AudioHelper::BGMVolume);
}
void BadScene::Terminate() {
    if (bgmInstance) {
        AudioHelper::StopSample(bgmInstance);
        bgmInstance.reset();  // safer and clearer
    }
    IScene::Terminate();
}
// void BadScene::BackOnClick() {
//    // Engine::GameEngine::GetInstance().ChangeScene("princess");
// }
void BadScene::NextOnClick(int stage) {
    AudioHelper::PlaySample("press.mp3");
    if (currentLine + 1 < (int)dialogueLines.size()) {
        currentLine++;
        dialogueLabel->Text = dialogueLines[currentLine];

        if (BadScreen) BadScreen->Visible = false;
        int screenW = Engine::GameEngine::GetInstance().GetScreenSize().x;
        int screenH = Engine::GameEngine::GetInstance().GetScreenSize().y;
        int halfW = screenW / 2;
        int halfH = screenH / 2;

        // // Default position and visibility
        // if (dialogueLabel)dialogueLabel->Position.x = halfW - 600;
        // if (dialogueLabel)dialogueLabel->Position.y = halfH - 100;
        // if (dialogueBoxImage)dialogueBoxImage->Visible = false;
        // if (rin_normal)rin_normal->Visible = false;
        // if (rin_worry)rin_worry->Visible = false;

        // // Hide black screen by default
        // if (blackScreen) blackScreen->Visible = false;

        // Special cases: Show dialogue box + appropriate Rin expression
        // if (currentLine == 4 || currentLine == 5 || currentLine == 12 || currentLine == 13) {
        //     if (dialogueLabel)dialogueLabel->Position.x = halfW - 300;
        //     if (dialogueLabel)dialogueLabel->Position.y = screenH - 150;
        //     if (dialogueBoxImage)dialogueBoxImage->Visible = true;
        //
        //     if (currentLine == 4 || currentLine == 5) {
        //         if (rin_worry)rin_worry->Visible = true;
        //     } else if (currentLine == 12 || currentLine == 13) {
        //         if (rin_normal)rin_normal->Visible = true;
        //     }
        // }

        // Show black screen on "You slowly approached the villager"
        // if (currentLine == 14) {
        //     if (blackScreen) blackScreen->Visible = true;
        //
        //     if (dialogueBoxImage)dialogueBoxImage->Visible = false;
        //     if (rin_normal)rin_normal->Visible = false;
        //     if (rin_worry)rin_worry->Visible = false;
        //
        //     // Position dialogue label at bottom center
        //     if (dialogueLabel)dialogueLabel->Position.x = halfW - 250;
        //     if (dialogueLabel)dialogueLabel->Position.y = screenH - 150;  // near bottom with some padding
        //
        //     // Optionally, make sure the label is visible if you hid it earlier
        //     if (dialogueLabel)dialogueLabel->Visible = true;
        // }
        if (currentLine == 9)
        {
            if (BadScreen) BadScreen->Visible = true;
        }

    } else {
        // PlayScene* scene = dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetScene("start"));
        // if (scene) {
        //     scene->MapId = stage;
        //     Engine::GameEngine::GetInstance().ChangeScene("start");
        // }
    }
}



void BadScene::BGMSlideOnValueChanged(float value) {
    AudioHelper::ChangeSampleVolume(bgmInstance, value);
    AudioHelper::BGMVolume = value;
}
void BadScene::SFXSlideOnValueChanged(float value) {
    AudioHelper::SFXVolume = value;
}