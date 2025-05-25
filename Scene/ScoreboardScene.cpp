// ScoreboardScene.cpp
#include "ScoreboardScene.hpp"
#include <fstream>
#include <sstream>
#include <ctime>
#include "Engine/GameEngine.hpp"
#include "Engine/AudioHelper.hpp"
#include "Engine/Resources.hpp"
#include "UI/Component/Label.hpp"
#include "UI/Component/ImageButton.hpp"
#include "StageSelectScene.hpp"
#include<iostream>

void ScoreboardScene::LoadScores() {
    scores.clear();
    std::ifstream fin("scoreboard.txt");
    if (!fin.is_open()) return; // Prevent crash if file is missing

    std::string name, date;
    int score;
    std::string line;
    while (std::getline(fin, line)) {
        std::istringstream ss(line);
        std::string name;
        int score;
        std::string date;
        ss >> name >> score;
        std::getline(ss >> std::ws, date);
        scores.emplace_back(name, score, date);
    }
    std::sort(scores.begin(), scores.end(), [](auto& a, auto& b) {
        return std::get<1>(a) > std::get<1>(b);
    });
    std::cerr << "[DEBUG] Loaded " << scores.size() << " scores.\n";

}

void ScoreboardScene::Initialize() {
    LoadScores();
    AddNavigationButtons(); // <-- Activate it!
    ShowPage(0); // show first page

    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;

    Engine::ImageButton *btn;

    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, halfH * 3 / 2 - 50, 400, 100);
    btn->SetOnClickCallback(std::bind(&ScoreboardScene::BackOnClick, this, 1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Back", "pirulen.ttf", 48, halfW, halfH * 3 / 2, 0, 0, 0, 255, 0.5, 0.5));

}

void ScoreboardScene::ShowPage(int pageNum) {
    // Remove only labels related to scoreboard (not buttons)
    for (auto it = objects.begin(); it != objects.end();) {
        Engine::IObject* obj = it->second;
        auto* label = dynamic_cast<Engine::Label*>(obj);
        //if (label && label != pageLabel) {
        if (label && label != pageLabel && label->Text.find("PREV") == std::string::npos && label->Text.find("NEXT") == std::string::npos && label->Text.find("Back") == std::string::npos) {
            delete obj;
            it = objects.erase(it);
        } else {
            ++it;
        }
    }
    page = pageNum;
    int start = page * entriesPerPage;
    int baseY = 120;
    int lineHeight = 50;

    AddNewObject(new Engine::Label("SCOREBOARD", "pirulen.ttf", 36, 800, 60, 255, 20, 147, 255, 0.5, 0));
    if (scores.empty()) {
        AddNewObject(new Engine::Label("No scores yet!", "pirulen.ttf", 28, 800, baseY, 255, 20, 147, 255, 0.5, 0));
    } else {
        for (int i = 0; i < entriesPerPage && (start + i) < scores.size(); ++i) {
            auto& [name, score, date] = scores[start + i];
            std::stringstream ss;
            ss << name << " - " << score << " (" << date << ")";
            AddNewObject(new Engine::Label(ss.str(), "pirulen.ttf", 28, 800, baseY + i * lineHeight, 139, 0, 98, 255, 0.5, 0));
        }
    }
    if (pageLabel) {
        pageLabel->Text = "PAGE: " + std::to_string(page + 1);
    }
    if (!pageLabel) {
        // Fallback: create it again (just in case)
        pageLabel = new Engine::Label("PAGE: ?", "pirulen.ttf", 28, 800, 680, 255, 255, 255, 255, 0.5, 0);
        std::cerr << "[ERROR] pageLabel was null during ShowPage!\n";
        AddNewObject(pageLabel);
    }
}
void ScoreboardScene::AddNavigationButtons() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;

    // prev button on
    Engine::ImageButton* prevBtn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png",
                                                           halfW - 700, halfH * 3 / 2 - 50, 400, 100);
    prevBtn->SetOnClickCallback([this]() {
        if (page > 0) ShowPage(page - 1);
    });
    AddNewControlObject(prevBtn);
    AddNewObject(new Engine::Label("PREV", "pirulen.ttf", 48,
        halfW - 500, halfH * 3 / 2, 0, 0, 0, 255, 0.5, 0.5));

    // nxt button
    Engine::ImageButton* nextBtn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png",
                                                           halfW + 300, halfH * 3 / 2 - 50, 400, 100);
    nextBtn->SetOnClickCallback([this]() {
        if ((page + 1) * entriesPerPage < scores.size()) ShowPage(page + 1);
    });
    AddNewControlObject(nextBtn);
    AddNewObject(new Engine::Label("NEXT", "pirulen.ttf", 48,
        halfW + 500, halfH * 3 / 2, 0, 0, 0, 255, 0.5, 0.5));
    // Page label
    pageLabel = new Engine::Label("PAGE: 1", "pirulen.ttf", 28, halfW, halfH * 3 / 2 + 120, 255, 255, 255, 255, 0.5, 0);
    pageLabel->Visible = true;
    AddNewObject(pageLabel);
}


void ScoreboardScene::Terminate() {
    if (bgmInstance) {
        AudioHelper::StopSample(bgmInstance);
        bgmInstance.reset();
    }

    // Manually delete only Label objects (to prevent double-deletion from ShowPage)
    for (auto it = objects.begin(); it != objects.end();) {
        Engine::IObject* obj = it->second;
        if (dynamic_cast<Engine::Label*>(obj)) {
            delete obj;
            it = objects.erase(it);
        } else {
            ++it;
        }
    }
    // Now let IScene handle the rest (buttons, internals)
    IScene::Terminate();
}


void ScoreboardScene::BackOnClick(int stage) {
    Engine::GameEngine::GetInstance().ChangeScene("stage-select");
}

