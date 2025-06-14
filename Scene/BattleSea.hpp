//
// Created by Shenice Mau on 6/14/2025.
//

#ifndef BATTLESEA_HPP
#define BATTLESEA_HPP




#include <memory>

#include "Engine/IScene.hpp"
#include <allegro5/allegro_audio.h>


class BattleSea final : public Engine::IScene {
private:

    std::vector<std::string> dialogueLines;
    Engine::Image* dialogueBoxImage = nullptr;
    Engine::Image* rin_battle = nullptr;
    Engine::Image* sea_battle = nullptr;
    Engine::Image* rin_close = nullptr;
    Engine::Sprite* blackScreen = nullptr;
    Group *UIGroup;
    Engine::Label *UIMoney;
    Engine::Label *UIKnowledge;
    Engine::Label *UISpeed;
    Engine::Label *UIStrength;
    Engine::Label *UILives;
    Engine::Label *EnemyLives;
    Engine::Label* playerHPLabel;
    Engine::Label* seaHPLabel;

    bool isDefending = false;





    int currentLine = 0;
    Engine::Label* dialogueLabel = nullptr;
    Engine::Label* rinHP = nullptr;
    Engine::Label* orcHP = nullptr;
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;
    ALLEGRO_TIMER* enemyAttackTimer = nullptr;
    ALLEGRO_EVENT_QUEUE* timerQueue = nullptr;
    bool enemyAttackScheduled = false;
    double enemyAttackStartTime = 0;
    Engine::Label* turnIndicatorLabel = nullptr;
    bool seaDefeatedShown = false;
    double defeatMessageStartTime = 0;
    Engine::Label* defeatLabel = nullptr;
    bool inputDisabled = false;







public:
    explicit BattleSea() = default;
    void Initialize() override;
    void Terminate() override;
    void BackOnClick(int stage);
    void NextOnClick(int stage);
    void BGMSlideOnValueChanged(float value);
    void SFXSlideOnValueChanged(float value);

    void OnClickAttack();
    void OnClickHeal();
    void OnClickDefend();
    void OnClickSkill();
    void EnemyTurn();
    void Update(float deltaTime)override;
};

#endif //BATTLESEA_HPP