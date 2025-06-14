//
// Created by Angelie Melisa on 2025/6/12.
//
#include "SaharaScene.hpp"

#include "GameData.hpp"
#include "Engine/AudioHelper.hpp"
#include "Enemy/ScorpionEnemy.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "Engine/LOG.hpp"
#include "Engine/Resources.hpp"
#include "Character/RinCharacter.hpp"
#include "Helper/Creature.hpp"
#include "Helper/House.hpp"

bool SaharaScene::IsTileWalkable(int tileType) const {
    return tileType == TILE_SAND;
}

PlayScene::TileType SaharaScene:: GetDefaultWalkableTile() const {
    return TILE_SAND;
}
void SaharaScene::Initialize() {
   // buat initialize stage pas masuk level
    // ini kayak setup awal: load map, load musuh, set UI,sm start bgms
    std::cout << "[DEBUG] Entering PlayScene::Initialize()\n";
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;    // ini kayak setup awal: load map, load musuh, set UI,sm start bgms
    float startX = halfW - 600;
    float startY = halfH - 100;
    keyUpDown = false;
    keyDownDown = false;
    keyLeftDown = false;
    keyRightDown = false;

    mapState.clear();
    keyStrokes.clear();
    ticks = 0;
    deathCountDown = -1;

    SpeedMult = 1;
    // Add groups from bottom to top.
    AddNewObject(TileMapGroup = new Engine::Group());
    AddNewObject(GroundEffectGroup = new Engine::Group());
    AddNewObject(DebugIndicatorGroup = new Engine::Group());
    AddNewObject(TowerGroup = new Engine::Group());
    AddNewObject(EnemyGroup = new Engine::Group());
    AddNewObject(BulletGroup = new Engine::Group());
    AddNewObject(EffectGroup = new Engine::Group());
    AddNewControlObject(UIGroup = new Engine::Group());

    int x_r = 1450;          // right side of the screen (adjust to your map)
    int y_start_r = 510;     // top
    int y_end_r = 1000;       // bottom
    int x_l = 28;          // right side of the screen (adjust to your map)
    int y_start_l = 300;     // top
    int y_end_l = 1000;
    int spacing_tree = 100;     // vertical gap between trees
    int scale_tree = 300;
    for (int y = y_start_l; y <= y_end_l; y += spacing_tree) {
        auto* tree = new Creature(x_l, y, "enemy/tree_sahara", 1, 5.0f, scale_tree, scale_tree);
        EffectGroup->AddNewObject(tree);
    }
    auto* mountain = new Creature(1128, 128, "enemy/mountain", 1, 5.0f, 320, 320);
    EffectGroup->AddNewObject(mountain);
    auto* mountain1 = new Creature(1000, 160, "enemy/mountain", 1, 5.0f, 320, 320);
    EffectGroup->AddNewObject(mountain1);
    auto* mountain2 = new Creature(1290, 200, "enemy/mountain", 1, 5.0f, 320, 320);
    EffectGroup->AddNewObject(mountain2);
    auto* mountain3 = new Creature(1000, 240, "enemy/mountain", 1, 5.0f, 320, 320);
    EffectGroup->AddNewObject(mountain3);
    auto* stalagmite4 = new Creature(1000, 400, "enemy/stalagmite", 1, 5.0f, 380, 380);
    EffectGroup->AddNewObject(stalagmite4);
    auto* stalagmite2 = new Creature(1128, 340, "enemy/stalagmite", 1, 5.0f, 380, 380);
    EffectGroup->AddNewObject(stalagmite2);
    auto* stalagmite3 = new Creature(1300, 350, "enemy/stalagmite", 1, 5.0f, 380, 380);
    EffectGroup->AddNewObject(stalagmite3);


    for (int y = y_start_r; y <= y_end_r; y += spacing_tree) {
        auto* tree = new Creature(x_r, y, "enemy/tree_sahara", 1, 5.0f, scale_tree, scale_tree);
        EffectGroup->AddNewObject(tree);
    }

    float sx = 512;
    float sy = 300;
    int spacing = 100;
    int scale = 270;

    // 3x3 grid offsets, excluding (0,0) where scorpion is
    std::vector<std::pair<int, int>> offsets = {
        {-2, -1}, { 0, -2}, { 2, -1},
        {-2,  0},           { 2,  0},
        {-2,  1},           { 2,  1},
    };

    for (const auto& [dx, dy] : offsets) {
        auto* stalagmite = new Creature(sx + dx * spacing, sy + dy * spacing, "enemy/stalagmite", 1, 5.0f, scale, scale);
        EffectGroup->AddNewObject(stalagmite);
    }

    if (GameData::scorpionHP > 0)
    {
        auto* scorpion_enemy = new ScorpionEnemy(
            512, 300,
            "enemy/scorpion",
            "scorpion");
        EffectGroup->AddNewObject(scorpion_enemy);
    }
    auto* rin = new RinCharacter(PlayScene::BlockSize / 2, PlayScene::BlockSize / 2);
    rin->SetPlayScene(this);
    EffectGroup->AddNewObject(rin);

    auto* tree1 = new Creature(130, 1000, "enemy/tree_sahara", 1, 5.0f, 320, 320);
    EffectGroup->AddNewObject(tree1);
    auto* tree = new Creature(256, 1000, "enemy/tree_sahara", 1, 5.0f, 320, 320);
    EffectGroup->AddNewObject(tree);
    auto* tree2 = new Creature(340, 1064, "enemy/tree_sahara", 1, 5.0f, 320, 320);
    EffectGroup->AddNewObject(tree2);
    auto* tree3 = new Creature(399, 1032, "enemy/tree_sahara", 1, 5.0f, 320, 320);
    EffectGroup->AddNewObject(tree3);
    auto* tree4 = new Creature(480, 1000, "enemy/tree_sahara", 1, 5.0f, 320, 320);
    EffectGroup->AddNewObject(tree4);
    auto* tree5 = new Creature(536, 1064, "enemy/tree_sahara", 1, 5.0f, 320, 320);
    EffectGroup->AddNewObject(tree5);
    auto* tree6 = new Creature(598, 1032, "enemy/tree_sahara", 1, 5.0f, 320, 320);
    EffectGroup->AddNewObject(tree6);

    auto* stalagmite = new Creature(110, 1110, "enemy/stalagmite", 1, 5.0f, 320, 320);
    EffectGroup->AddNewObject(stalagmite);
    auto* stalagmite1 = new Creature(700, 1078, "enemy/stalagmite", 1, 5.0f, 320, 320);
    EffectGroup->AddNewObject(stalagmite1);



    ReadMap();
    ReadEnemyWave();
    mapDistance = CalculateBFSDistance();
    ConstructUI();
    imgTarget = new Engine::Image("play/target.png", 0, 0);
    imgTarget->Visible = false;
    preview = nullptr;
    UIGroup->AddNewObject(imgTarget);
    // Preload Lose Scene
    deathBGMInstance = Engine::Resources::GetInstance().GetSampleInstance("astronomia.ogg");
    Engine::Resources::GetInstance().GetBitmap("lose/benjamin-happy.png");
    // Start BGM.
    //bgmId = AudioHelper::PlayBGM("desert-explore.mp3");
}
std::string SaharaScene::GetMapImagePath() const {
    return "play/sahara_map.png";
}
void SaharaScene::CloseMap() {
    PlayScene::CloseMap();  // Optional: call base logic

    // Add village-specific logic here if needed
    std::cout << "[VillageScene] Closed village map.\n";
}

void SaharaScene::ReadMap() {
    // load file map.txt jd mapState
    // ngegambar tile floor / dirt ke TileMapGroup
    std::string filename = std::string("Resource/map_sahara") + std::to_string(MapId) + ".txt";
    // Read map file.
    char c;
    std::vector<int> mapData;
    std::ifstream fin(filename);
    if (!fin.is_open()) {
        std::cerr << "Failed to open map file: " << filename << "\n";
        throw std::ios_base::failure("Cannot open map file");
    }
    while (fin >> c) {
        switch (c) {
            case '0': mapData.push_back(0); break;
            case '1': mapData.push_back(1); break;
            case '2': mapData.push_back(2); break;
            case '3': mapData.push_back(3); break;
            case '4': mapData.push_back(4); break;
            case '5': mapData.push_back(5); break;
            case '6': mapData.push_back(6); break;
            case '7': mapData.push_back(7); break;
            case '8': mapData.push_back(8); break;
            case '9': mapData.push_back(9); break;

            case '\n':
            case '\r':
                if (static_cast<int>(mapData.size()) / MapWidth != 0)
                    throw std::ios_base::failure("Map data is corrupted.");
                break;
            default: throw std::ios_base::failure("Map data is corrupted.");
        }
    }
    fin.close();
    // Validate map data.
    if (static_cast<int>(mapData.size()) != MapWidth * MapHeight)
        throw std::ios_base::failure("Map data is corrupted.");
    // Store map in 2d array.
    mapState = std::vector<std::vector<TileType>>(MapHeight, std::vector<TileType>(MapWidth));
    for (int i = 0; i < MapHeight; i++) {
        for (int j = 0; j < MapWidth; j++) {
            const int num = mapData[i * MapWidth + j];
            //mapState[i][j] = num ? TILE_FLOOR : TILE_DIRT;
            // if (1) {
            //     TileMapGroup->AddNewObject(new Engine::Image("play/floor.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize)); // previously floor.png
            // }
            // if (2) {
            //     TileMapGroup->AddNewObject(new Engine::Image("play/grass.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
            // }
            switch(num) {
                case 0: mapState[i][j] = TILE_GRASS;
                    TileMapGroup->AddNewObject( new Engine::Image("play/grass.png", j*BlockSize, i*BlockSize, BlockSize, BlockSize) );
                    break;
                case 1: mapState[i][j] = TILE_FLOOR;
                    TileMapGroup->AddNewObject(new Engine::Image("play/floor.png", j*BlockSize, i*BlockSize, BlockSize, BlockSize));
                    break;
                case 2: mapState[i][j] = TILE_FENCE;
                    TileMapGroup->AddNewObject(new Engine::Image("play/fence.png", j*BlockSize, i*BlockSize, BlockSize, BlockSize));
                    break;
                case 3: mapState[i][j] = TILE_FENCE_RIGHT;
                    TileMapGroup->AddNewObject(new Engine::Image("play/fence_right.png", j*BlockSize, i*BlockSize, BlockSize, BlockSize));
                    break;
                case 4: mapState[i][j] = TILE_FENCE_DOWN;
                    TileMapGroup->AddNewObject(new Engine::Image("play/fence_down.png", j*BlockSize, i*BlockSize, BlockSize, BlockSize));
                    break;
                case 5: mapState[i][j] = TILE_FENCE_LEFT;
                    TileMapGroup->AddNewObject(new Engine::Image("play/fence_left.png", j*BlockSize, i*BlockSize, BlockSize, BlockSize));
                    break;
                case 6: mapState[i][j] = TILE_BOOK;
                    TileMapGroup->AddNewObject(new Engine::Image("play/house_book.png", j*BlockSize, i*BlockSize, BlockSize, BlockSize));
                    break;
                case 7: mapState[i][j] = TILE_INVENTORY;
                    TileMapGroup->AddNewObject(new Engine::Image("play/house_inventory.png", j*BlockSize, i*BlockSize, BlockSize, BlockSize));
                    break;
                case 8: mapState[i][j] = TILE_WATER;
                    TileMapGroup->AddNewObject(new Engine::Image("play/water.png", j*BlockSize, i*BlockSize, BlockSize, BlockSize));
                    break;
                case 9: mapState[i][j] = TILE_SAND;
                    TileMapGroup->AddNewObject(new Engine::Image("play/gurun.png", j*BlockSize, i*BlockSize, BlockSize, BlockSize));
                    break;

                    //case 2: mapState[i][j] = TILE_GRASS;
                    //TileMapGroup->AddNewObject(new Engine::Image("play/grass.png", j*BlockSize, i*BlockSize, BlockSize, BlockSize) );
                    //break;
                //case 3: mapState[i][j] = TILE_TREE;
                   // TileMapGroup->AddNewObject(new Engine::Image("play/tree.png", j*BlockSize, i*BlockSize, BlockSize, BlockSize) );
                   // break;
                //default:
                    //break;
            }
        }
    }
}
void SaharaScene::Update(float deltaTime) {
    PlayScene::Update(deltaTime);
    Transition();
}

void SaharaScene::Transition() {
    if (_didTransition) return;

    // find our RinCharacter in the scene
    RinCharacter* rin = nullptr;
    for (auto* obj : EffectGroup->GetObjects()) {
        if ((rin = dynamic_cast<RinCharacter*>(obj))) break;
    }
    if (!rin) return;

    // grid‐coords:
    int gx = static_cast<int>(rin->Position.x) / BlockSize;
    int gy = static_cast<int>(rin->Position.y) / BlockSize;

    // if she's on bottom‐right, switch scenes once
    if (gx == MapWidth - 1 && gy == MapHeight - 1) {
        _didTransition = true;
        Engine::GameEngine::GetInstance().ChangeScene("ocean");
    }
}
