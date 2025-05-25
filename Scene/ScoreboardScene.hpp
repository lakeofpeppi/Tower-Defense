// ScoreboardScene.hpp
#ifndef SCOREBOARDSCENE_HPP
#define SCOREBOARDSCENE_HPP
#pragma once
#include <allegro5/allegro_audio.h>
#include <memory>
#include <vector>
#include <string>
#include "Engine/IScene.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"

class ScoreboardScene final : public Engine::IScene {
private:
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;
    std::vector<std::tuple<std::string, int, std::string>> scores; // name, score, date
    int page = 0;
    const int entriesPerPage = 5;
    Engine::Label* pageLabel = nullptr;
    void LoadScores();
    void AddBackButton();
    void AddNavigationButtons();
    void ShowPage(int pageNum);
public:
    explicit ScoreboardScene() = default;
    void BackOnClick(int stage);
    void Initialize() override;
    void Terminate() override;
};

#endif