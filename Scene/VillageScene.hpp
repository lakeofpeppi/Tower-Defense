#ifndef VILLAGESCENE_HPP
#define VILLAGESCENE_HPP

#include "PlayScene.hpp"
#include "Character/RinCharacter.hpp"
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
    bool _didTransition = false;
    RinCharacter* rin = nullptr;

public:

    void Initialize() override;
    bool IsTileWalkable(int tileType) const override ;
    void ReadMap() override;
    TileType GetDefaultWalkableTile() const override;
    void ShowDialogue(const std::vector<std::string>& lines);
    void AdvanceDialogue();
    void OnKeyDown(int keyCode) override;
    void OnKeyUp(int keyCode) override;
    void Transition() override;
    void Terminate()override;
    void Update(float deltaTime) override;
    bool footstepsPlaying = false;
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> footstepsInstance;
    bool IsPlayerInBounds();
    std::string GetMapImagePath() const override;
    void CloseMap() override;







    bool dialogueActive = false;
    int currentDialogueIndex = 0;



    void HideDialogue();
};

#endif
//hello