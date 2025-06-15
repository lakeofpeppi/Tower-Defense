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
#include "NormalScene.hpp"
#include <iostream>
#include <ostream>

#include "PlayScene.hpp"

void NormalScene::Initialize() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;

    Engine::ImageButton *btn;


    int wpic = 1900;
    int hpic = 1300;
    AddNewObject(new Engine::Image("play/black.png", 0, 0, wpic, hpic));

    dialogueLines = {
        "                      You chose to leave her be", //0
        "                     You have failed your mission", //1
        "             However, you chose to honour Sayuki's choice.", //2
        "   You chose to not return to the village, and continued your journey",//3
        "       And so the residents in Miyamura village remains waiting",//4
        "             for the outlander to bring back their Princess", //5
        "           they will never know that you won't come back", //6
        "", //7
        "                            NORMAL ENDING ", //5
    };

    float lineHeight = 70;
    float startX = halfW - 600;
    float startY = halfH - 100;

    NormalScreen = new Engine::Image("play/normal end.png", 0, 0, w, h);
    NormalScreen->Visible = false;
    AddNewObject(NormalScreen);



    dialogueLabel = new Engine::Label(dialogueLines[0], "To The Point.ttf", 70, halfW - 500, h - 608, 255, 255, 255, 255, 0.0, 0.5);
    AddNewObject(dialogueLabel);

    int btnX = 1792 - 400 - 20;
    int btnY = 1216 - 100 - 20;

    //next
    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW + 680, halfH + 500, 200, 80);
    btn->SetOnClickCallback(std::bind(&NormalScene::NextOnClick, this, 1));
    AddNewControlObject(btn);

    AddNewObject(new Engine::Label("next", "To The Point.ttf", 80,
                                   btnX + 300,
                                   btnY + 50,
                                   255, 255, 255, 255,
                                   0.5, 0.5));

    bgmInstance = AudioHelper::PlaySample("princess-meet.mp3", true, AudioHelper::BGMVolume);
}
void NormalScene::Terminate() {
    if (bgmInstance) {
        AudioHelper::StopSample(bgmInstance);
        bgmInstance.reset();
    }
    IScene::Terminate();
}

void NormalScene::NextOnClick(int stage) {
    AudioHelper::PlaySample("press.mp3");
    if (currentLine + 1 < (int)dialogueLines.size()) {
        currentLine++;
        dialogueLabel->Text = dialogueLines[currentLine];

        if (NormalScreen) NormalScreen->Visible = false;
        int screenW = Engine::GameEngine::GetInstance().GetScreenSize().x;
        int screenH = Engine::GameEngine::GetInstance().GetScreenSize().y;
        int halfW = screenW / 2;
        int halfH = screenH / 2;


        if (currentLine == 7)
        {
            if (NormalScreen) NormalScreen->Visible = true;
        }

    } else {
        }
}



void NormalScene::BGMSlideOnValueChanged(float value) {
    AudioHelper::ChangeSampleVolume(bgmInstance, value);
    AudioHelper::BGMVolume = value;
}
void NormalScene::SFXSlideOnValueChanged(float value) {
    AudioHelper::SFXVolume = value;
}