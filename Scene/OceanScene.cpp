#include "OceanScene.hpp"
#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "Engine/LOG.hpp"
#include "Engine/Resources.hpp"
#include "Character/RinCharacter.hpp"
#include "Helper/House.hpp"
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


    // Create house
    /*
    auto* Inventory = new House(
        1184, 928,
        "play/house_inventory.png",
        "intro");
    EffectGroup->AddNewObject(Inventory);

    auto* Book = new House(
        288, 512,  // Example position
        "play/house_book.png",  // Image of the house
        "book"         // The scene it should go to when touched
    );
    EffectGroup->AddNewObject(Book);


    auto* npcTalker = new NPC(
        512, 928,
        "npc/npc_idle",
        "intro");
    EffectGroup->AddNewObject(npcTalker);
    */

    auto* rin = new RinCharacter(PlayScene::BlockSize / 2, PlayScene::BlockSize / 2);
    rin->SetPlayScene(this);
    EffectGroup->AddNewObject(rin);


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