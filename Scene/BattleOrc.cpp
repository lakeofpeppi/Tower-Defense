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

void BattleOrc::Initialize() {
    enemyAttackTimer = al_create_timer(0.7); // 0.7 second delay
    timerQueue = al_create_event_queue();
    al_register_event_source(timerQueue, al_get_timer_event_source(enemyAttackTimer));


    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;


    Engine::ImageButton *btn;


    int wpic = 1900;
    int hpic = 1300;
    AddNewObject(new Engine::Image("play/bg_forrestBattle.png", 0, 0, wpic, hpic));
    //AddNewObject(new Engine::Image("play/rin dialogue expressions.png", 0, 620, 2700, 600));

    // Create the first line label
    float lineHeight = 70;
    float startX = halfW - 600;
    float startY = halfH - 100;
    // Black screen
    // Black screen

    blackScreen = new Engine::Sprite("play/black.png", 1792 / 2.0f, 1216 / 2.0f, 1712, 1136);
    blackScreen->Tint = al_map_rgba(255, 255, 255, 128); // 50% opacity
    AddNewObject(blackScreen);




    // Rin normal
    rin_battle = new Engine::Image("play/rin battle.png", halfW - 950, h - 900, 1350, 900);
    AddNewObject(rin_battle);

    // Rin worried
    orc_battle = new Engine::Image("play/orc battle.png", halfW - 250, h - 1200, 1500, 1000);
    AddNewObject(orc_battle);

    ///ADA SEGMENTATION ERROR KL PAKE INI
    // UIGroup->AddNewObject(UILives = new Engine::Label(std::string("HP: ") + std::to_string(GameData::lives), "pirulen.ttf", 32, 1500, 300));
    // UIGroup->AddNewObject(EnemyLives = new Engine::Label(std::string("Enemy HP:") + std::to_string(GameData::orcHP), "pirulen.ttf", 24, 1486, 550));
    //


    int btnWidth = 320; // Increased from 250
    int btnHeight = 100;
    int padding = 20;
    int baseX = 1792 - btnWidth * 2 - padding * 2 - 50;
    int baseY = 1216 - btnHeight * 2 - padding * 2 - 50;


    // ATTACK button
    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png",
        baseX, baseY, btnWidth, btnHeight);
    btn->SetOnClickCallback(std::bind(&BattleOrc::OnClickAttack, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("ATTACK", "pirulen.ttf", 40,
        baseX + btnWidth / 2, baseY + btnHeight / 2,
        255, 255, 255, 255, 0.5, 0.5));

    // HEAL button
    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png",
        baseX + btnWidth + padding, baseY, btnWidth, btnHeight);
    btn->SetOnClickCallback(std::bind(&BattleOrc::OnClickHeal, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("HEAL", "pirulen.ttf", 40,
        baseX + btnWidth + padding + btnWidth / 2, baseY + btnHeight / 2,
        255, 255, 255, 255, 0.5, 0.5));

    // ESCAPE button
    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png",
        baseX, baseY + btnHeight + padding, btnWidth, btnHeight);
    btn->SetOnClickCallback(std::bind(&BattleOrc::OnClickDefend, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("DEFEND", "pirulen.ttf", 40,
        baseX + btnWidth / 2, baseY + btnHeight + padding + btnHeight / 2,
        255, 255, 255, 255, 0.5, 0.5));

    // SKILL button
    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png",
        baseX + btnWidth + padding, baseY + btnHeight + padding, btnWidth, btnHeight);
    btn->SetOnClickCallback(std::bind(&BattleOrc::OnClickSkill, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("SKILL", "pirulen.ttf", 40,
        baseX + btnWidth + padding + btnWidth / 2, baseY + btnHeight + padding + btnHeight / 2,
        255, 255, 255, 255, 0.5, 0.5));

    playerHPLabel = new Engine::Label(
    std::string("HP: ") + std::to_string(GameData::lives),
    "pirulen.ttf", 32,
    halfW - 950 + 200, h - 900 - 50,
    255, 255, 255, 255, 0.0, 1.0); // Left aligned, top
    AddNewObject(playerHPLabel);

    orcHPLabel = new Engine::Label(
     std::string("Enemy HP: ") + std::to_string(GameData::orcHP),
     "pirulen.ttf", 32,
     halfW - 250 + 300, h - 1050, // shifted lower by 50
     255, 0, 0, 255, 0.0, 1.0); // Left aligned, top
    AddNewObject(orcHPLabel);



    turnIndicatorLabel = new Engine::Label(
    "YOUR TURN!", "pirulen.ttf", 48,
    1792 / 2, 50, // center top
    255, 255, 255, 255, 0.5, 0.5 // yellow text, centered
);
    AddNewObject(turnIndicatorLabel);


    bgmInstance = AudioHelper::PlaySample("fight.ogg", true, AudioHelper::BGMVolume);
}
void BattleOrc::OnClickAttack() {
    if (inputDisabled) return;
    AudioHelper::PlaySample("slash.ogg");
    GameData::orcHP -= GameData::strength;
    orcHPLabel->Text = std::string("Enemy HP: ") + std::to_string(GameData::orcHP);
    if (!enemyAttackScheduled) {
        enemyAttackScheduled = true;
        enemyAttackStartTime = al_get_time();
        turnIndicatorLabel->Text = "ENEMY TURN!";
        turnIndicatorLabel->Color = al_map_rgba(255, 0, 0, 255);

    }


}

void BattleOrc::OnClickHeal() {
    if (inputDisabled) return;
    //AudioHelper::PlaySample("collect.mp3");
    GameData::lives += 20; // No upper cap
    playerHPLabel->Text = std::string("HP: ") + std::to_string(GameData::lives);
    if (!enemyAttackScheduled) {
        enemyAttackScheduled = true;
        enemyAttackStartTime = al_get_time();
        turnIndicatorLabel->Text = "ENEMY TURN!";
        turnIndicatorLabel->Color = al_map_rgba(255, 0, 0, 255);
    }


}

void BattleOrc::OnClickDefend() {
    if (inputDisabled) return;
    //AudioHelper::PlaySample("press.mp3");
    isDefending = true;
    // You would use this flag in the enemy's attack logic like so:
    // int damage = isDefending ? GameData::orcStrength / 2 : GameData::orcStrength;
    // GameData::lives -= damage;
    if (!enemyAttackScheduled) {
        enemyAttackScheduled = true;
        enemyAttackStartTime = al_get_time();
        turnIndicatorLabel->Text = "ENEMY TURN!";
        turnIndicatorLabel->Color = al_map_rgba(255, 0, 0, 255);
    }


}


void BattleOrc::OnClickSkill() {
    if (inputDisabled) return;
    AudioHelper::PlaySample("slash.ogg");
    GameData::orcHP -= (GameData::strength + 50);
    GameData::lives -= 5;

    orcHPLabel->Text = std::string("Enemy HP: ") + std::to_string(GameData::orcHP);
    playerHPLabel->Text = std::string("HP: ") + std::to_string(GameData::lives);
    if (!enemyAttackScheduled) {
        enemyAttackScheduled = true;
        enemyAttackStartTime = al_get_time();
        turnIndicatorLabel->Text = "ENEMY TURN!";
        turnIndicatorLabel->Color = al_map_rgba(255, 0, 0, 255);
    }


}

void BattleOrc::Terminate() {
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
        bgmInstance.reset();  // safer and clearer
    }
    IScene::Terminate();
}
void BattleOrc::EnemyTurn() {

    int damage = isDefending ? GameData::orcStrength / 2 : GameData::orcStrength;
    GameData::lives -= damage;
    playerHPLabel->Text = std::string("HP: ") + std::to_string(GameData::lives);
    isDefending = false;
    std::cout << "[DEBUG] EnemyTurn called. Damage: " << damage << ", lives left: " << GameData::lives << "\n";

}

void BattleOrc::BackOnClick(int stage) {

}
void BattleOrc::NextOnClick(int stage) {

}



void BattleOrc::BGMSlideOnValueChanged(float value) {
    AudioHelper::ChangeSampleVolume(bgmInstance, value);
    AudioHelper::BGMVolume = value;
}
void BattleOrc::SFXSlideOnValueChanged(float value) {
    AudioHelper::SFXVolume = value;
}
void BattleOrc::Update(float deltaTime) {
    Engine::IScene::Update(deltaTime);

    if (enemyAttackScheduled) {
        std::cout << "[DEBUG] update(): enemyAttackScheduled=" << enemyAttackScheduled << "\n";
        double currentTime = al_get_time();
        if (currentTime - enemyAttackStartTime >= 2) {
            //AudioHelper::PlaySample("growl.mp3");
            //AudioHelper::PlaySample("growl.ogg");
            EnemyTurn();
            enemyAttackScheduled = false;
            turnIndicatorLabel->Text = "YOUR TURN!";
            turnIndicatorLabel->Color = al_map_rgba(255, 255, 255, 255);
        }
    }

    // Check if orc was just defeated
    // Check if orc was just defeated
    if (GameData::orcHP <= 0 && !orcDefeatedShown) {
        GameData::orcHP = 0;
        orcDefeatedShown = true;
        inputDisabled = true; // Disable inputs
        defeatMessageStartTime = al_get_time();

        // Top label: YOU HAVE DEFEATED AN ORC
        defeatLabel = new Engine::Label(
            "YOU HAVE DEFEATED AN ORC",
            "pirulen.ttf", 48,
            1792 / 2, 1216 / 2 - 40,
            255, 255, 255, 255, 0.5, 0.5
        );
        AddNewObject(defeatLabel);

        // Bottom label: BONE SHOVEL OBTAINED!
        AddNewObject(new Engine::Label(
            "BONE SHOVEL OBTAINED!",
            "pirulen.ttf", 36,
            1792 / 2, 1216 / 2 + 40,
            255, 255, 255, 255, 0.5, 0.5
        ));

        AudioHelper::StopSample(bgmInstance);
        AudioHelper::PlaySample("win.wav");
    }

    // After 5 seconds, switch scene
    if (orcDefeatedShown && al_get_time() - defeatMessageStartTime > 5.0) {
        Engine::GameEngine::GetInstance().ChangeScene("forest");
    }

}
