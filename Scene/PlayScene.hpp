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
#include "UI/Component/ImageButton.hpp"




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

    // int lives = 50;
    // int money = 0;
    int SpeedMult;
    // int knowledge = 0;
    // int strength = 100;
    // int speed = 100;

    Engine::Label* cheatLabel = nullptr;
    float cheatTimer = 0.0f;
    bool cheatActive = false;
    Engine::Label* refundLabel = nullptr;
    float refundTimer = 0.0f;
    bool refundActive = false;

    //for invincibility to catch lizard
    bool invincible = false;
    double invincibleStartTime = 0;
    Engine::Label* InvincibleLabel = nullptr;

    double poisonStingStartTime = 0;
    Engine::Label* poisonStingLabel = nullptr;

    //for null emotion so that when fighting sea creature your hp wont drain
    bool null_emotion = false;
    double null_emotionStartTime = 0;
    Engine::Label* null_emotionLabel = nullptr;



    std::list<std::pair<bool, Engine::IObject*>>::iterator mapOverlayIterator;
    std::list<std::pair<bool, Engine::IControl*>>::iterator closeMapCtrlIterator;
    std::list<std::pair<bool, Engine::IObject*>>::iterator closeMapObjIterator;


    bool isBoneTurretPlaced;






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
        TILE_SOIL,
        TILE_ROSE,
        TILE_DIG,
    };
    virtual bool IsTileWalkable(int tileType) const = 0;

    bool keyUpDown    = false;
    bool keyDownDown  = false;
    bool keyLeftDown  = false;
    bool keyRightDown = false;




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
    Engine::Image *blackScreen = nullptr;
    Engine::Image* mapOverlay = nullptr;
    Engine::Image* rin_prof = nullptr;
    bool mapVisible = false;


    Engine::ImageButton* closeMapBtn = nullptr;
    Engine::Label* closeMapLabel = nullptr;



    Engine::ImageButton* saveBtn = nullptr;
    Engine::ImageButton* titleBtn = nullptr;
    Engine::ImageButton* settingsBtn = nullptr;
    Engine::ImageButton* backBtn = nullptr;

    Engine::Label* saveLabel = nullptr;
    Engine::Label* titleLabel = nullptr;
    Engine::Label* settingsLabel = nullptr;
    Engine::Label* backLabel = nullptr;



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
    void MenuOnClick();
    void Hit();
    void SaveProgressOnClick();
    void ReturnToTitleOnClick();
    void SettingsOnClick();
    void BackOnClick();
    void LoadProgress();
    virtual void MapOnClick();
    virtual void CloseMap();

    virtual std::string GetMapImagePath() const;




    int GetMoney() const;
    void EarnMoney(int money);
    void GainKnowledge();
    void LoseStrength();
    void LoseSpeed();
    void BuyHP();
    void GoToIntroScene();


    void ConstructUI();
    virtual void UIBtnClicked(int id);
    bool CheckSpaceValid(int x, int y, int turret = -1);
    //void AddMultipleFlashes(int count, float interval);
    virtual TileType GetDefaultWalkableTile() const;
    virtual void Transition() = 0;


    std::vector<std::vector<int>> CalculateBFSDistance();
    // void ModifyReadMapTiles();
    //std::vector<std::pair<float, std::function<void()>>> scheduledEffects;

};
#endif   // PLAYSCENE_HPP
