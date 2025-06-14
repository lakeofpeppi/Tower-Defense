//
// Created by Hsuan on 2024/4/10.
//

#include "StartScene.h"
#include <allegro5/allegro_audio.h>
#include <functional>
#include <memory>
#include <string>

#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Point.hpp"
#include "Engine/Resources.hpp"
#include "PlayScene.hpp"
#include "Scene/StartScene.h"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "UI/Component/Slider.hpp"
#include "GameData.hpp"

// TODO HACKATHON-2 (1/3): You can imitate the 2 files: 'StartScene.hpp', 'StartScene.cpp' to implement your SettingsScene.(DONE)
void StartScene::Initialize() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    Engine::ImageButton *btn;

    int wpic = 1811;
    int hpic = 1245;
    AddNewObject(new Engine::Image("play/title screen.png", 0, 0, wpic, hpic));

    AddNewObject(new Engine::Label("The Outlander's Mission", "To The Point.ttf", 200, halfW+250, halfH / 3 +150,  255, 255, 255, 255, 0.5, 0.5));

    btn = new Engine::ImageButton("stage-select/underline.png", "stage-select/underline.png", halfW + 100, halfH / 2 + 200, 400, 100);
    btn->SetOnClickCallback(std::bind(&StartScene::PlayOnClick, this, 1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("start new game", "To The Point.ttf", 100, halfW + 300, halfH / 2 + 250, 0, 0, 0, 255, 0.5, 0.5));

    btn = new Engine::ImageButton("stage-select/underline.png", "stage-select/underline.png", halfW + 100, halfH * 3 / 2 - 250, 400, 100);
    btn->SetOnClickCallback(std::bind(&StartScene::StageOnClick, this, 2));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("continue game", "To The Point.ttf", 100, halfW + 300, halfH * 3 / 2 - 200, 0, 0, 0, 255, 0.5, 0.5));

    btn = new Engine::ImageButton("stage-select/underline.png", "stage-select/underline.png", halfW + 550, halfH * 3 / 2 - 850, 300, 100);
    btn->SetOnClickCallback(std::bind(&StartScene::SettingsOnClick, this, 3));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("settings", "To The Point.ttf", 100, halfW + 700, halfH * 3 / 2 - 800, 250, 250, 250, 255, 0.5, 0.5));

}
void StartScene::Terminate() {
    IScene::Terminate();
}
void StartScene::PlayOnClick(int stage) {
    GameData::Reset();

    Engine::GameEngine::GetInstance().ChangeScene("intro");
}
void StartScene::StageOnClick(int stage) {
    PlayScene* scene = dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetScene("village"));
    if (scene) {
        scene->MapId = stage;
        scene->LoadProgress();
        Engine::GameEngine::GetInstance().ChangeScene("village");
    };


}
void StartScene::SettingsOnClick(int stage) {
    Engine::GameEngine::GetInstance().ChangeScene("settings");
}