#ifndef PLAYSCENE_HPP
#define PLAYSCENE_HPP
#include <allegro5/allegro_audio.h>
#include <list>
#include <memory>
#include <utility>
#include <vector>

#include "Engine/IScene.hpp"
#include "Engine/Point.hpp"




class Turret;
namespace Engine {
    class Group;
    class Image;
    class Label;
    class Sprite;
}   // namespace Engine




class PlayScene final : public Engine::IScene {
    ALLEGRO_SAMPLE_ID bgmId;
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> deathBGMInstance;

// test to push

protected:
    int lives;
    int money;
    int SpeedMult;
    Engine::Label* cheatLabel = nullptr;
    float cheatTimer = 0.0f;
    bool cheatActive = false;
    Engine::Label* refundLabel = nullptr;
    float refundTimer = 0.0f;
    bool refundActive = false;




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
        TILE_FLOOR,
        TILE_OCCUPIED,
        TILE_TREE,
        TILE_FENCE,
        TILE_FENCE_DOWN,
        TILE_FENCE_RIGHT,
        TILE_FENCE_LEFT,
        TILE_BOOK,
        TILE_INVENTORY
    };

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
    Engine::Label *UILives;
    Engine::Image *imgTarget;
    Engine::Sprite *dangerIndicator;
    //shovel handling

    Turret *preview;
    std::vector<std::vector<TileType>> mapState;
    std::vector<std::vector<Turret*>> mapTurret;
    std::vector<std::vector<int>> mapDistance;
    std::list<std::pair<int, float>> enemyWaveData;
    std::list<int> keyStrokes;
    static Engine::Point GetClientSize();
    explicit PlayScene() = default;
    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void Draw() const override;
    void OnMouseDown(int button, int mx, int my) override;
    void OnMouseMove(int mx, int my) override;
    void OnMouseUp(int button, int mx, int my) override;
    void OnKeyDown(int keyCode) override;
    void OnKeyUp(int keyCode) override;
    void Hit();
    int GetMoney() const;
    void EarnMoney(int money);
    void ReadMap();
    void ReadEnemyWave();
    void ConstructUI();
    void UIBtnClicked(int id);
    bool CheckSpaceValid(int x, int y);
    //void AddMultipleFlashes(int count, float interval);

    std::vector<std::vector<int>> CalculateBFSDistance();
    // void ModifyReadMapTiles();
    //std::vector<std::pair<float, std::function<void()>>> scheduledEffects;

};
#endif   // PLAYSCENE_HPP
