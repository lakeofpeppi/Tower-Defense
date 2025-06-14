//
// Created by Angelie Melisa on 2025/6/12.
//
//
// Created by Angelie Melisa on 2025/6/12.
//
#include "ForestScene.hpp"

#include "GameData.hpp"
#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "Engine/LOG.hpp"
#include "Engine/Resources.hpp"
#include "Character/RinCharacter.hpp"
#include "Helper/House.hpp"
#include "Enemy/OrcEnemy.hpp"
#include "Enemy/FrogEnemy.hpp"
#include "Helper/Creature.hpp"


bool ForestScene::IsTileWalkable(int tileType) const {
    return tileType == TILE_SOIL;
}

PlayScene::TileType ForestScene:: GetDefaultWalkableTile() const {
    return TILE_SOIL;
}
void ForestScene::Initialize() {
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
    // Should support buttons.
    AddNewControlObject(UIGroup = new Engine::Group());

    int x_r = 1480;          // right side of the screen (adjust to your map)
    int y_start_r = 100;     // top
    int y_end_r = 1000;       // bottom
    int x_l = 55;          // right side of the screen (adjust to your map)
    int y_start_l = 290;     // top
    int y_end_l = 1256;
    int spacing_tree = 100;     // vertical gap between trees
    int scale_tree = 256;
    int y_top = 55;               // y-position for the top row
    int x_start_top = 300;        // leave a little margin on the left
    int x_end_top = 1400;
    for (int x = x_start_top; x <= x_end_top; x += spacing_tree) {
        auto* topBush = new Creature(x, y_top, "enemy/bushes", 1, 5.0f, scale_tree, scale_tree);
        EffectGroup->AddNewObject(topBush);
    }
    for (int y = y_start_l; y <= y_end_l; y += spacing_tree) {
        auto* tree = new Creature(x_l, y, "enemy/bushes", 1, 5.0f, scale_tree, scale_tree);
        EffectGroup->AddNewObject(tree);
    }
    for (int y = y_start_r; y <= y_end_r; y += spacing_tree) {
        auto* tree = new Creature(x_r, y, "enemy/bushes", 1, 5.0f, scale_tree, scale_tree);
        EffectGroup->AddNewObject(tree);
    }

    //auto* mountain = new Creature(700, 128, "enemy/mountain_green", 1, 5.0f, 320, 320);
    //EffectGroup->AddNewObject(mountain);
    float ox = 1200;
    float oy = 400;
    int spacing = 100;
    int bush_scale = 150;


    if (GameData::orcHP > 0)
    {
        auto* orc_enemy = new OrcEnemy(
                    ox, oy,
                    "enemy/orc",
                    "orc");
        EffectGroup->AddNewObject(orc_enemy);
    }


    std::vector<std::pair<int, int>> bush_offsets = {
        {-2, -1}, { -1, -1.5}, { -1, -2},{ 0, -2}, {1,-2},
        {-2,  0},
        {-2,  1},           { 1,  1},
        { -2,  2},{ 0,  2},{ 1,  2},
        { -2,  3},  { 0,  3},{ 1,  3},
    };

    for (const auto& [dx, dy] : bush_offsets) {
        auto* redBush = new Creature(ox + dx * spacing, oy + dy * spacing, "enemy/bushes_red", 1, 5.0f, bush_scale, bush_scale);
        EffectGroup->AddNewObject(redBush);
    }



    float jx = 350.f;
    float jy = 750.f;


    // Offsets surrounding the frog (excluding the center {0, 0})
    std::vector<std::pair<float, float>> bush_yellow = {
        {-1, -1}, { 0, -1}, { 1, -1},
        {-1,  0},    {0,0}  ,     { 1,  0},
        {-1,  1}, { 0,  1}, { 1,  1},
    };

    // Correct loop with the correct vector
    for (auto [dx, dy] : bush_yellow) {
        float bx = jx + dx * spacing;
        float by = jy + dy * spacing;

        auto* yellowBush = new Creature(
            bx, by,
            "enemy/lava",       // path to your bush sprite
            1, 5.0f,
            bush_scale, bush_scale
        );
        EffectGroup->AddNewObject(yellowBush);
    }

    auto* frog_enemy = new FrogEnemy(
             jx, jy,
            "enemy/frog",
            "intro");
    EffectGroup->AddNewObject(frog_enemy);

    /*
    if (frog_enemy->isdisappearing) {
        auto* frog_die = new Creature(jx, jy, "enemy/clam_frame", 4, 50);
        EffectGroup->AddNewObject(frog_die);
    }
    */
    /*
    auto* frog_disappear = new FrogEnemy(
             300, 800,
            "enemy/frog",
            "intro");
    EffectGroup->AddNewObject(frog_disappear);
*/
    auto* rin = new RinCharacter(PlayScene::BlockSize / 2, PlayScene::BlockSize / 2);
    rin->SetPlayScene(this);
    EffectGroup->AddNewObject(rin);

    if (GameData::returnX != -1 && GameData::returnY != -1) {
        rin->Position.x = GameData::returnX;
        rin->Position.y = GameData::returnY;
        GameData::returnX = -1;
        GameData::returnY = -1;
    }
    auto* tree = new Creature(135, 1060, "enemy/treee", 1, 5.0f, 320, 320);
    EffectGroup->AddNewObject(tree);
    auto* tree2 = new Creature(384, 1060, "enemy/treeee", 1, 5.0f, 320, 320);
    EffectGroup->AddNewObject(tree2);
    auto* tree3 = new Creature(512, 1060, "enemy/treeeee", 1, 5.0f, 320, 320);
    EffectGroup->AddNewObject(tree3);
    auto* tree4 = new Creature(768, 1060, "enemy/treeeeee", 1, 5.0f, 320, 320);
    EffectGroup->AddNewObject(tree4);
    auto* bush = new Creature(900, 200, "enemy/bush", 1, 5.0f, 320, 320);
    EffectGroup->AddNewObject(bush);
    auto* bush1 = new Creature(128, 1256, "enemy/pinklava", 1, 5.0f, 320, 320);
    EffectGroup->AddNewObject(bush1);
    auto* bush2 = new Creature(256, 1256, "enemy/pinklava", 1, 5.0f, 320, 320);
    EffectGroup->AddNewObject(bush2);
    auto* lava = new Creature(945, 200, "enemy/lava", 1, 5.0f, 320, 320);
    EffectGroup->AddNewObject(lava);

    ReadMap();
    ReadEnemyWave();
    mapDistance = CalculateBFSDistance();
    ConstructUI();
    imgTarget = new Engine::Image("play/target.png", 0, 0);
    imgTarget->Visible = false;
    preview = nullptr;
    UIGroup->AddNewObject(imgTarget);
    deathBGMInstance = Engine::Resources::GetInstance().GetSampleInstance("astronomia.ogg");
    Engine::Resources::GetInstance().GetBitmap("lose/benjamin-happy.png");
    bgmId = AudioHelper::PlayBGM("forest-explore.wav");
}
std::string ForestScene::GetMapImagePath() const {
    return "play/forest_map.png";
}
void ForestScene::CloseMap() {
    PlayScene::CloseMap();
    std::cout << "[VillageScene] Closed village map.\n";
}

void ForestScene::ReadMap() {
    std::string filename = std::string("Resource/map_forest") + std::to_string(MapId) + ".txt";
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
            case 'a': mapData.push_back(10); break;
            case '\n':
            case '\r':
                if (static_cast<int>(mapData.size()) / MapWidth != 0)
                    throw std::ios_base::failure("Map data is corrupted.");
                break;
            default: throw std::ios_base::failure("Map data is corrupted.");
        }
    }
    fin.close();
    if (static_cast<int>(mapData.size()) != MapWidth * MapHeight)
        throw std::ios_base::failure("Map data is corrupted.");
    mapState = std::vector<std::vector<TileType>>(MapHeight, std::vector<TileType>(MapWidth));
    for (int i = 0; i < MapHeight; i++) {
        for (int j = 0; j < MapWidth; j++) {
            const int num = mapData[i * MapWidth + j];
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
                case 10: mapState[i][j] = TILE_SOIL;
                    TileMapGroup->AddNewObject(new Engine::Image("play/soil.png", j*BlockSize, i*BlockSize, BlockSize, BlockSize));
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
void ForestScene::Update(float deltaTime) {
    PlayScene::Update(deltaTime);
    Transition();
}

void ForestScene::Transition() {
    if (_didTransition) return;

    RinCharacter* rin = nullptr;
    for (auto* obj : EffectGroup->GetObjects()) {
        if ((rin = dynamic_cast<RinCharacter*>(obj))) break;
    }
    if (!rin) return;

    int gx = static_cast<int>(rin->Position.x) / BlockSize;
    int gy = static_cast<int>(rin->Position.y) / BlockSize;

    if (gx == MapWidth - 1 && gy == MapHeight - 1) {
        _didTransition = true;
        Engine::GameEngine::GetInstance().ChangeScene("sahara");
    }
}
