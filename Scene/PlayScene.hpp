#ifndef PLAYSCENE_HPP
#define PLAYSCENE_HPP
#include <allegro5/allegro_audio.h>
#include <list>
#include <memory>
#include <utility>
#include <vector>
//#include "BasePlayScene.hpp"

#include "Engine/IScene.hpp"
#include "Engine/Point.hpp"




class Turret;
namespace Engine {
    class Group;
    class Image;
    class Label;
    class Sprite;
}   // namespace Engine




class PlayScene : public Engine::IScene  {



// test to push

protected:
    ALLEGRO_SAMPLE_ID bgmId;
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> deathBGMInstance;

    int lives = 50;
    int money = 0;
    int SpeedMult;
    int knowledge = 0;
    int strength = 100;
    int speed = 100;
    bool invincible = false;
    double invincibleStartTime = 0;
    Engine::Label* cheatLabel = nullptr;
    float cheatTimer = 0.0f;
    bool cheatActive = false;
    Engine::Label* refundLabel = nullptr;
    float refundTimer = 0.0f;
    bool refundActive = false;
    Engine::Label* InvincibleLabel = nullptr;


    Engine::Image* rin_prof = nullptr;







public:
    enum class PlayMode {
        NONE,
        BUILD_TURRET,
        SHOVEL
    };


    PlayMode Mode = PlayMode::NONE;
    Engine::Sprite* shovelPreview = nullptr;
    enum TileType {
        TILE_GRASS,
        TILE_WATER,
        TILE_FLOOR,
        TILE_OCCUPIED,
        TILE_TREE,
        TILE_SAND,
        TILE_FENCE,
        TILE_FENCE_DOWN,
        TILE_FENCE_RIGHT,
        TILE_FENCE_LEFT,
        TILE_BOOK,
        TILE_INVENTORY,
        TILE_SOIL
    };
    virtual bool IsTileWalkable(int tileType) const = 0;

    bool keyUpDown    = false;
    bool keyDownDown  = false;
    bool keyLeftDown  = false;
    bool keyRightDown = false;

    /*
    enum class PlayMode {
        NONE,
        BUILD_TURRET,
        SHOVEL
    };
    PlayMode Mode = PlayMode::NONE;
    */
    // void ShowDialogue(const std::vector<std::string>& lines);
    // void AdvanceDialogue();



    // int currentDialogueIndex = 0;



    // void HideDialogue();
    static bool DebugMode;
    static const std::vector<Engine::Point> directions;
    static const int MapWidth, MapHeight;
    static const int BlockSize;
    static const float DangerTime;
    static const Engine::Point SpawnGridPoint;
    static const Engine::Point EndGridPoint;
    static const std::vector<int> code;
    int MapId;
    float ticks;
    float deathCountDown;
    // Map tiles.
    Group *TileMapGroup;
    Group *GroundEffectGroup;
    Group *DebugIndicatorGroup;
    Group *BulletGroup;
    Group *TowerGroup;
    Group *EnemyGroup;
    Group *EffectGroup;
    Group *UIGroup;
    Engine::Label *UIMoney;
    Engine::Label *UIKnowledge;
    Engine::Label *UISpeed;
    Engine::Label *UIStrength;
    Engine::Label *UILives;
    Engine::Image *imgTarget;
    Engine::Sprite *dangerIndicator;
    //shovel handling



    std::vector<std::vector<int>> mapDistance;
    std::vector<std::vector<TileType>> mapState;
    //bool IsTileWalkable(int tileType) const = 0;

    virtual void ReadMap() = 0;
    void ReadEnemyWave();
    Turret *preview;
    std::vector<std::vector<Turret*>> mapTurret;
    std::list<std::pair<int, float>> enemyWaveData;
    std::list<int> keyStrokes;
    static Engine::Point GetClientSize();
    explicit PlayScene() = default;
    virtual void Initialize() = 0;
    void Terminate();
    virtual void Update(float deltaTime) ;
    void Draw() const ;
    void OnMouseDown(int button, int mx, int my) ;
    void OnMouseMove(int mx, int my);
    void OnMouseUp(int button, int mx, int my) ;
    void OnKeyDown(int keyCode);
    void OnKeyUp(int keyCode);
    void Hit();


    int GetMoney() const;
    void EarnMoney(int money);
    void GainKnowledge();
    void LoseStrength();
    void LoseSpeed();
    void BuyHP();


    void ConstructUI();
    void UIBtnClicked(int id);
    bool CheckSpaceValid(int x, int y);
    //void AddMultipleFlashes(int count, float interval);
    virtual TileType GetDefaultWalkableTile() const;
    virtual void Transition() = 0;

    std::vector<std::vector<int>> CalculateBFSDistance();
    // void ModifyReadMapTiles();
    //std::vector<std::pair<float, std::function<void()>>> scheduledEffects;

};
#endif   // PLAYSCENE_HPP
