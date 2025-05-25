#include <functional>
#include <string>
#include <fstream>
#include <iomanip>
#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Point.hpp"
#include "PlayScene.hpp"
#include "UI/Component/Image.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "UI/Component/TextBox.hpp"
#include "WinScene.hpp"
#include <iostream>

void WinScene::Initialize() {
    std::cout << "WinScene::Initialize() called." << std::endl;
    ticks = 0;
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;

    AddNewObject(new Engine::Image("win/benjamin-sad.png", halfW, halfH, 0, 0, 0.5, 0.5));
    AddNewObject(new Engine::Label("You Win!", "pirulen.ttf", 48, halfW, halfH / 4, 255, 255, 255, 255, 0.5, 0.5));

    /*
    auto* playScene = dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetScene("play"));
    if (playScene) {
        finalScore = playScene->GetMoney();
    }
    */

    nameInput = new Engine::TextBox("PLAYER", "pirulen.ttf", 36, halfW, halfH / 4 + 60, 255, 255, 255, 255, 0.5, 0.5);
    AddNewControlObject(nameInput);

    Engine::ImageButton* btn = new Engine::ImageButton("win/dirt.png", "win/floor.png", halfW - 200, halfH + 240, 400, 100);
    btn->SetOnClickCallback(std::bind(&WinScene::BackOnClick, this, 2));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Back", "pirulen.ttf", 48, halfW, halfH + 290, 0, 0, 0, 255, 0.5, 0.5));
    bgmId = AudioHelper::PlaySample("win.wav", false, AudioHelper::BGMVolume);
}

void WinScene::Terminate() {
    IScene::Terminate();
    AudioHelper::StopSample(bgmId);
}

void WinScene::Update(float deltaTime) {
    ticks += deltaTime;
    if (ticks > 4 && ticks < 100) {
        // play BGM langsung azza without checking the previous scene
        ticks = 100;
    bgmId = AudioHelper::PlaySample("happy.ogg", true, AudioHelper::BGMVolume);
    }
}


void WinScene::BackOnClick(int stage) {
    std::string name = nameInput ? nameInput->Text : "unknown";
    if (name.empty()) name = "unknown";

    std::ofstream fout("scoreboard.txt", std::ios::app);
    if (fout.is_open()) {
        std::time_t now = std::time(nullptr);
        std::string date = std::ctime(&now);
        date.pop_back(); // remove newline
        fout << name << " " << finalScore << " " << date << "\n";
        fout.close();
    } else {
        std::cerr << "[ERROR] Failed to open scoreboard.txt\n";
    }

    Engine::GameEngine::GetInstance().ChangeScene("stage-select");
}

void WinScene::SetFinalScore(int score) {
    finalScore = score;
}

void WinScene::OnKeyDown(int keyCode) {
    if (nameInput)
        nameInput->OnKeyDown(keyCode);
}
