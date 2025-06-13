//
// Created by Angelie Melisa on 2025/6/14.
//

#include <functional>
#include <memory>
#include <string>

#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Point.hpp"
#include "Engine/Resources.hpp"
#include "PlayScene.hpp"
#include "StageSelectScene.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "UI/Component/Slider.hpp"

void StageSelectScene::Initialize() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    Engine::ImageButton *btn;

    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, halfH / 2 - 50, 400, 100);
    btn->SetOnClickCallback(std::bind(&StageSelectScene::PlayOnClick, this, 1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Attack", "pirulen.ttf", 48, halfW, halfH / 2, 0, 0, 0, 255, 0.5, 0.5));

    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, halfH / 2 + 75, 400, 100);
    btn->SetOnClickCallback(std::bind(&StageSelectScene::Ending, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Defend", "pirulen.ttf", 48, halfW, halfH / 2 + 125, 0, 0, 0, 255, 0.5, 0.5));

    /*
    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, halfH * 3 / 2 - 50, 400, 100);
    btn->SetOnClickCallback(std::bind(&StageSelectScene::BackOnClick, this, 1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Back", "pirulen.ttf", 48, halfW, halfH * 3 / 2, 0, 0, 0, 255, 0.5, 0.5));

    // for scoreboard_scene
    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, halfH + 30, 400, 100);
    btn->SetOnClickCallback(std::bind(&StageSelectScene::ScoreboardOnClick, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Scoreboard", "pirulen.ttf", 36, halfW, halfH + 80, 0, 0, 0, 255, 0.5, 0.5));
*/
    // Not safe if release resource while playing, however we only free while change scene, so it's fine.
    bgmInstance = AudioHelper::PlaySample("title.wav", true, AudioHelper::BGMVolume);

}