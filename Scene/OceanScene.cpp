#include "OceanScene.hpp"
#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "Engine/LOG.hpp"
#include "Engine/Resources.hpp"
#include "Character/RinCharacter.hpp"
#include "Helper/House.hpp"
#include "Enemy/CrabEnemy.hpp"
#include "Enemy/JellyFishEnemy.hpp"
#include "Enemy/SharkEnemy.hpp"
#include "Helper/Creature.hpp"

#include "Helper/NPC.hpp"

bool OceanScene::IsTileWalkable(int tileType) const {
    return tileType == TILE_WATER; // only grass is walkable
}
PlayScene::TileType OceanScene::GetDefaultWalkableTile() const {
    return TILE_WATER;
}
void OceanScene::Initialize() {
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

    auto* rin = new RinCharacter(PlayScene::BlockSize / 2, PlayScene::BlockSize / 2);
    rin->SetPlayScene(this);
    EffectGroup->AddNewObject(rin);

    auto* yellow4 = new Creature(200, 500, "enemy/anemon_yellow", 1, 5.0f, 320, 320);
    EffectGroup->AddNewObject(yellow4);
    auto* yellow5 = new Creature(300, 590, "enemy/anemon_yellow", 1, 5.0f, 320, 320);
    EffectGroup->AddNewObject(yellow5);
    auto* yellow6 = new Creature(400, 500, "enemy/anemon_yellow", 1, 5.0f, 320, 320);
    EffectGroup->AddNewObject(yellow6);
    auto* yellow7 = new Creature(500, 590, "enemy/anemon_yellow", 1, 5.0f, 320, 320);
    EffectGroup->AddNewObject(yellow7);
    auto* yellow = new Creature(200, 700, "enemy/anemon_yellow", 1, 5.0f, 320, 320);
    EffectGroup->AddNewObject(yellow);
    auto* yellow1 = new Creature(300, 790, "enemy/anemon_yellow", 1, 5.0f, 320, 320);
    EffectGroup->AddNewObject(yellow1);
    auto* yellow2 = new Creature(400, 700, "enemy/anemon_yellow", 1, 5.0f, 320, 320);
    EffectGroup->AddNewObject(yellow2);
    auto* yellow3 = new Creature(500, 790, "enemy/anemon_yellow", 1, 5.0f, 320, 320);
    EffectGroup->AddNewObject(yellow3);

    auto* clam = new Creature(228, 1064, "enemy/clam_frame", 3, 0.7f);
    EffectGroup->AddNewObject(clam);

    auto* anemon = new Creature(330, 1064, "enemy/pink_anemon", 3, 0.7f);
    EffectGroup->AddNewObject(anemon);
    auto* anemon1 = new Creature(400, 1090, "enemy/pink_anemon", 3, 0.7f);
    EffectGroup->AddNewObject(anemon1);
    auto* anemon2 = new Creature(470, 1032, "enemy/pink_anemon", 3, 0.7f);
    EffectGroup->AddNewObject(anemon2);

    int x_r = 1450;          // right side of the screen (adjust to your map)
    int y_start_r = 100;     // top
    int y_end_r = 1000;       // bottom
    int x_l = 55;          // right side of the screen (adjust to your map)
    int y_start_l = 290;     // top
    int y_end_l = 1256;
    int spacing_tree = 100;     // vertical gap between trees
    int scale_tree = 180;
    int x_start = 1120;         // far left
    int x_end = 1600;        // far right (adjust to map width)
    int y_bottom = 1150;

    for (int y = y_start_l; y <= y_end_l; y += spacing_tree) {
        auto* tree = new Creature(x_l, y, "enemy/rock_left", 1, 5.0f, scale_tree, scale_tree);
        EffectGroup->AddNewObject(tree);
    }
    for (int y = y_start_r; y <= y_end_r; y += spacing_tree) {
        auto* tree = new Creature(x_r, y, "enemy/rock_left", 1, 5.0f, scale_tree, scale_tree);
        EffectGroup->AddNewObject(tree);
    }

    for (int x = x_start; x <= x_end; x += spacing_tree) {
        auto* seaweed_yellow = new Creature(x, y_bottom, "enemy/anemon_yellow", 1, 5.0f, scale_tree, scale_tree);
        EffectGroup->AddNewObject(seaweed_yellow);
    }





    float ox = 1100;
    float oy = 400;
    int spacing = 100;
    int bush_scale = 150;


    auto* jellyfish_enemy = new JellyFishEnemy(
        ox, oy,
        "enemy/jellyfish_frame",
        "intro");
    EffectGroup->AddNewObject(jellyfish_enemy);

    std::vector<std::pair<int, int>> seaweed_offsets = {
        {-1, -1},                   { 1, -1},{ 2, -1},
          {-1,  0},              { 1,  0}, { 2,  0},
          {-1,  1}, { 0,  1}, { 1,  1},{ 2,  1},
        {-1,  2}, { 0,  2}, { 1,  2},{ 2,  2},
            {-1,  3}, { 0,  3}, { 1,  3},{ 2,  3},
    };

    for (const auto& [dx, dy] : seaweed_offsets) {
        auto* redBush = new Creature(ox + dx * spacing, oy + dy * spacing, "enemy/seaweed", 1, 5.0f, bush_scale, bush_scale);
        EffectGroup->AddNewObject(redBush);
    }
    /*std::vector<std::pair<int, int>> seaweed_border = {
        {-2, -2}, {-1, -2}, {0, -2}, {1, -2}, {2, -2}, {3, -2},
        {-2, -1}, {0, -1}, {3, -1},
        {-2,  0}, {0,  0}, {3,  0},
        {-2,  1}, {3,  1},
        {-2,  2}, {3,  2},
        {-2,  3}, {3,  3},
        {-1,  4}, {0,  4}, {1,  4}, {2,  4}, {3,  4},
    };

    for (const auto& [dx, dy] : seaweed_border) {
        auto* seaweed_yellow = new Creature(ox + dx * spacing, oy + dy * spacing, "enemy/anemon_yellow", 1, 5.0f, bush_scale, bush_scale);
        EffectGroup->AddNewObject(seaweed_yellow);
    }
    */
    float jx = 750;
    float jy = 1064;

    auto* crab_enemy = new CrabEnemy(
      jx, jy,
      "enemy/crab",
      "intro");
    EffectGroup->AddNewObject(crab_enemy);


    /*std::vector<std::pair<int, int>> seaweed_yellow_offsets = {
        {-1, -2},         {0, -2},      { 1, -2},
        {-1, -1},         {0, -1},      { 1, -1},
          {-1,  0},
          {-1,  1}, { 0,  1}, { 1,  1},
        {-1,  2}, { 0,  2}, { 1,  2},

    };
    */
    std::vector<std::pair<int, int>> seaweed_yellow_offsets = {
         {-2,  1},  {-1,  1}, { 0,  1}, { 1,  1}, { 2,  1},{ 3,  1},
    };

    for (const auto& [dx, dy] : seaweed_yellow_offsets) {
        auto* seaweed_yellow = new Creature(jx + dx * spacing, jy + dy * spacing, "enemy/seaweed", 1, 5.0f, bush_scale, bush_scale);
        EffectGroup->AddNewObject(seaweed_yellow);
    }

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
    bgmId = AudioHelper::PlayBGM("ocean-explore.wav");
}
std::string OceanScene::GetMapImagePath() const {
    return "play/ocean_map.png";
}
void OceanScene::CloseMap() {
    PlayScene::CloseMap();  // Optional: call base logic

    // Add village-specific logic here if needed
    std::cout << "[VillageScene] Closed village map.\n";
}

void OceanScene::ReadMap() {
    // load file map.txt jd mapState
    // ngegambar tile floor / dirt ke TileMapGroup
    std::string filename = std::string("Resource/map_sea") + std::to_string(MapId) + ".txt";
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
void OceanScene::Update(float deltaTime) {
    PlayScene::Update(deltaTime);
    Transition();
}

void OceanScene::Transition() {

}