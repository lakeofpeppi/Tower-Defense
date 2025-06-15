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

#include <iostream>
#include <ostream>

#include "PlayScene.hpp"

void GoodScene::Initialize() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;

    Engine::ImageButton *btn;


    int wpic = 1900;
    int hpic = 1300;
    AddNewObject(new Engine::Image("play/black.png", 0, 0, wpic, hpic));

    dialogueLines = {
        "          You chose to stay with her", //0
        "         You have failed your mission", //1
        "   However in exchange for a hefty reward.", //2
        "You led a peaceful and happy life with Sayuki",//3
        "Free from people's expectations and burdens",//4
        "", //5
        "              GOOD ENDING ",
    };

    float lineHeight = 70;
    float startX = halfW - 600;
    float startY = halfH - 100;
    GoodScreen = new Engine::Image("play/good end.png", 0, 0, w, h);
    GoodScreen->Visible = false;
    AddNewObject(GoodScreen);



    dialogueLabel = new Engine::Label(dialogueLines[0], "To The Point.ttf", 70, halfW - 300, h - 608, 255, 255, 255, 255, 0.0, 0.5);
    AddNewObject(dialogueLabel);

    int btnX = 1792 - 400 - 20;
    int btnY = 1216 - 100 - 20;

    //next
    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW + 680, halfH + 500, 200, 80);
    btn->SetOnClickCallback(std::bind(&GoodScene::NextOnClick, this, 1));
    AddNewControlObject(btn);

    AddNewObject(new Engine::Label("next", "To The Point.ttf", 80,
                                   btnX + 300,
                                   btnY + 50,
                                   255, 255, 255, 255,
                                   0.5, 0.5));


    bgmInstance = AudioHelper::PlaySample("princess-meet.mp3", true, AudioHelper::BGMVolume);
}
void GoodScene::Terminate() {
    if (bgmInstance) {
        AudioHelper::StopSample(bgmInstance);
        bgmInstance.reset();
    }
    IScene::Terminate();
}

void GoodScene::NextOnClick(int stage) {
    AudioHelper::PlaySample("press.mp3");
    if (currentLine + 1 < (int)dialogueLines.size()) {
        currentLine++;
        dialogueLabel->Text = dialogueLines[currentLine];
        if (GoodScreen) GoodScreen->Visible = false;

        int screenW = Engine::GameEngine::GetInstance().GetScreenSize().x;
        int screenH = Engine::GameEngine::GetInstance().GetScreenSize().y;
        int halfW = screenW / 2;
        int halfH = screenH / 2;
        if (currentLine == 5)
        {
            if (GoodScreen) GoodScreen->Visible = true;
        }


    } else {
        }
}



void GoodScene::BGMSlideOnValueChanged(float value) {
    AudioHelper::ChangeSampleVolume(bgmInstance, value);
    AudioHelper::BGMVolume = value;
}
void GoodScene::SFXSlideOnValueChanged(float value) {
    AudioHelper::SFXVolume = value;
}