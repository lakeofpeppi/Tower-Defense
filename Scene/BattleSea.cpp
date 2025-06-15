//
// Created by Shenice Mau on 6/14/2025.
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
#include <iostream>
#include <ostream>
#include "BattleOrc.hpp"
#include "GameData.hpp"
#include "PlayScene.hpp"
#include "Engine/Sprite.hpp"
#include "Scene/BattleScorpion.hpp"
#include "Scene/BattleSea.hpp"

void BattleSea::Initialize() {
    enemyAttackTimer = al_create_timer(0.7);
    timerQueue = al_create_event_queue();
    al_register_event_source(timerQueue, al_get_timer_event_source(enemyAttackTimer));


    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;


    Engine::ImageButton *btn;


    int wpic = 1900;
    int hpic = 1300;
    AddNewObject(new Engine::Image("play/bg_seaBattle.png", 0, 0, wpic, hpic));

    float lineHeight = 70;
    float startX = halfW - 600;
    float startY = halfH - 100;

    blackScreen = new Engine::Sprite("play/black.png", 1792 / 2.0f, 1216 / 2.0f, 1712, 1136);
    blackScreen->Tint = al_map_rgba(255, 255, 255, 128); // 50% opacity
    AddNewObject(blackScreen);



    // Rin normal
    rin_battle = new Engine::Image("play/rin battle.png", halfW - 950, h - 900, 1350, 900);
    AddNewObject(rin_battle);

    // Rin worried
    sea_battle = new Engine::Image("play/jellyfishBattle.png", halfW - 250, h - 1200, 1000, 1000);
    AddNewObject(sea_battle);



    int btnWidth = 320;
    int btnHeight = 100;
    int padding = 20;
    int baseX = 1792 - btnWidth * 2 - padding * 2 - 50;
    int baseY = 1216 - btnHeight * 2 - padding * 2 - 50;


    // ATTACK button
    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png",
        baseX, baseY, btnWidth, btnHeight);
    btn->SetOnClickCallback(std::bind(&BattleSea::OnClickAttack, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("ATTACK", "pirulen.ttf", 40,
        baseX + btnWidth / 2, baseY + btnHeight / 2,
        255, 255, 255, 255, 0.5, 0.5));

    // HEAL button
    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png",
        baseX + btnWidth + padding, baseY, btnWidth, btnHeight);
    btn->SetOnClickCallback(std::bind(&BattleSea::OnClickHeal, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("HEAL", "pirulen.ttf", 40,
        baseX + btnWidth + padding + btnWidth / 2, baseY + btnHeight / 2,
        255, 255, 255, 255, 0.5, 0.5));

    // ESCAPE button
    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png",
        baseX, baseY + btnHeight + padding, btnWidth, btnHeight);
    btn->SetOnClickCallback(std::bind(&BattleSea::OnClickDefend, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("DEFEND", "pirulen.ttf", 40,
        baseX + btnWidth / 2, baseY + btnHeight + padding + btnHeight / 2,
        255, 255, 255, 255, 0.5, 0.5));

    // SKILL button
    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png",
        baseX + btnWidth + padding, baseY + btnHeight + padding, btnWidth, btnHeight);
    btn->SetOnClickCallback(std::bind(&BattleSea::OnClickSkill, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("SKILL", "pirulen.ttf", 40,
        baseX + btnWidth + padding + btnWidth / 2, baseY + btnHeight + padding + btnHeight / 2,
        255, 255, 255, 255, 0.5, 0.5));

    playerHPLabel = new Engine::Label(
    std::string("HP: ") + std::to_string(GameData::lives),
    "pirulen.ttf", 32,
    halfW - 950 + 200, h - 900 - 50,
    255, 255, 255, 255, 0.0, 1.0);
    AddNewObject(playerHPLabel);

    seaHPLabel = new Engine::Label(
     std::string("Enemy HP: ") + std::to_string(GameData::seaHP),
     "pirulen.ttf", 32,
     halfW - 250 + 300, h - 1050,
     255, 0, 0, 255, 0.0, 1.0);
    AddNewObject(seaHPLabel);



    turnIndicatorLabel = new Engine::Label(
    "YOUR TURN!", "pirulen.ttf", 48,
    1792 / 2, 50,
    255, 255, 255, 255, 0.5, 0.5
);
    AddNewObject(turnIndicatorLabel);


    Engine::ImageButton* backBtn = new Engine::ImageButton(
        "stage-select/dirt.png",
        "stage-select/floor.png",
        20, 20,
        200, 60);
    backBtn->SetOnClickCallback([]() {
        AudioHelper::PlaySample("press.mp3");
        Engine::GameEngine::GetInstance().ChangeScene("ocean");
    });
    AddNewControlObject(backBtn);
    AddNewObject(new Engine::Label("BACK", "pirulen.ttf", 32,
        20 + 100, 20 + 30,
        255, 255, 255, 255, 0.5, 0.5));
    bgmInstance = AudioHelper::PlaySample("fight.ogg", true, AudioHelper::BGMVolume);
}
void BattleSea::OnClickAttack() {
    if (inputDisabled) return;

    AudioHelper::PlaySample("slash.mp3");
    GameData::seaHP -= GameData::strength;
    if (GameData::seaHP < 0) GameData::seaHP = 0;

    if (!GameData::isNull) {
        GameData::lives -= 5;
        if (GameData::lives < 0) GameData::lives = 0;
    }

    seaHPLabel->Text = std::string("Enemy HP: ") + std::to_string(GameData::seaHP);
    playerHPLabel->Text = std::string("HP: ") + std::to_string(GameData::lives);
}



void BattleSea::OnClickHeal() {
    if (inputDisabled) return;
    AudioHelper::PlaySample("collect.mp3");
    GameData::lives += 20;
    playerHPLabel->Text = std::string("HP: ") + std::to_string(GameData::lives);
    if (!enemyAttackScheduled) {
        enemyAttackScheduled = true;
        enemyAttackStartTime = al_get_time();
        turnIndicatorLabel->Text = "ENEMY TURN!";
        turnIndicatorLabel->Color = al_map_rgba(255, 0, 0, 255);
    }


}

void BattleSea::OnClickDefend() {
    if (inputDisabled) return;
    AudioHelper::PlaySample("press.mp3");
    isDefending = true;
    if (!enemyAttackScheduled) {
        enemyAttackScheduled = true;
        enemyAttackStartTime = al_get_time();
        turnIndicatorLabel->Text = "ENEMY TURN!";
        turnIndicatorLabel->Color = al_map_rgba(255, 0, 0, 255);
    }


}


void BattleSea::OnClickSkill() {
    if (inputDisabled) return;
    if (!GameData::poisonStingEquip) return;


    AudioHelper::PlaySample("slash.mp3");
    GameData::seaHP -= (GameData::strength + 50);
    if (GameData::seaHP < 0) GameData::seaHP = 0;

    if (!GameData::isNull) {
        GameData::lives -= 5;
        if (GameData::lives < 0) GameData::lives = 0;
    }

    seaHPLabel->Text = std::string("Enemy HP: ") + std::to_string(GameData::seaHP);
    playerHPLabel->Text = std::string("HP: ") + std::to_string(GameData::lives);
}

void BattleSea::Terminate() {
    if (enemyAttackTimer) {
        al_destroy_timer(enemyAttackTimer);
        enemyAttackTimer = nullptr;
    }
    if (timerQueue) {
        al_destroy_event_queue(timerQueue);
        timerQueue = nullptr;
    }

    if (bgmInstance) {
        AudioHelper::StopSample(bgmInstance);
        bgmInstance.reset();
    }
    IScene::Terminate();
}
void BattleSea::EnemyTurn() {

    int damage = isDefending ? GameData::seaStrength / 2 : GameData::seaStrength;
    GameData::lives -= damage;
    playerHPLabel->Text = std::string("HP: ") + std::to_string(GameData::lives);
    isDefending = false;
    std::cout << "[DEBUG] EnemyTurn called. Damage: " << damage << ", lives left: " << GameData::lives << "\n";

}

void BattleSea::BackOnClick(int stage) {

}
void BattleSea::NextOnClick(int stage) {

}

void BattleSea::BGMSlideOnValueChanged(float value) {
    AudioHelper::ChangeSampleVolume(bgmInstance, value);
    AudioHelper::BGMVolume = value;
}
void BattleSea::SFXSlideOnValueChanged(float value) {
    AudioHelper::SFXVolume = value;
}
void BattleSea::Update(float deltaTime) {
    Engine::IScene::Update(deltaTime);

    if (enemyAttackScheduled) {
        std::cout << "[DEBUG] update(): enemyAttackScheduled=" << enemyAttackScheduled << "\n";
        double currentTime = al_get_time();
        if (currentTime - enemyAttackStartTime >= 2) {
            EnemyTurn();
            enemyAttackScheduled = false;
            turnIndicatorLabel->Text = "YOUR TURN!";
            turnIndicatorLabel->Color = al_map_rgba(255, 255, 255, 255);
        }
    }

    if (GameData::seaHP <= 0 && !seaDefeatedShown) {
        GameData::seaHP = 0;
        seaDefeatedShown = true;
        inputDisabled = true;
        defeatMessageStartTime = al_get_time();


        defeatLabel = new Engine::Label(
            "YOU HAVE KILLED AN INNOCENT CREATURE",
            "pirulen.ttf", 48,
            1792 / 2, 1216 / 2,
            255, 255, 255, 255, 0.5, 0.5
        );
        AddNewObject(defeatLabel);

        AudioHelper::StopSample(bgmInstance);
        AudioHelper::PlaySample("win.wav");
    }
        if (seaDefeatedShown && al_get_time() - defeatMessageStartTime > 5.0) {
            Engine::GameEngine::GetInstance().ChangeScene("princess");
    }


}
