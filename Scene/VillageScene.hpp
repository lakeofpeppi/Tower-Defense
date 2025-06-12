#ifndef VILLAGESCENE_HPP
#define VILLAGESCENE_HPP

#include "PlayScene.hpp"
class VillageScene : public PlayScene {
    Engine::Label* dialogueLabel = nullptr;
    std::vector<std::string> dialogueLines;

    Engine::Image* rin_normal = nullptr;
    Engine::Image* rin_worry = nullptr;
    Engine::Image* rin_close = nullptr;
    Engine::Image* toma_happy = nullptr;
    Engine::Image* toma_shock = nullptr;
    Engine::Image* toma_worry = nullptr;
    Engine::Image* dialogueBoxImage = nullptr;

public:

    void Initialize() override;
    bool IsTileWalkable(int tileType) const override ;
    virtual void ReadMap() override;
    void ShowDialogue(const std::vector<std::string>& lines);
    void AdvanceDialogue();
    void OnKeyDown(int keyCode) override;
    void OnKeyUp(int keyCode) override;



    bool dialogueActive = false;
    int currentDialogueIndex = 0;



    void HideDialogue();
};

#endif