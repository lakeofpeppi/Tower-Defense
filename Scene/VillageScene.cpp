#include "VillageScene.hpp"
#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "Engine/LOG.hpp"
#include "Engine/Resources.hpp"
#include "Character/RinCharacter.hpp"
#include "Helper/House.hpp"
#include "Helper/NPC.hpp"
#include "UI/Component/Label.hpp"
#include <allegro5/allegro.h>
#include "UI/Animation/Plane.hpp"

bool VillageScene::IsTileWalkable(int tileType) const {
    return tileType == TILE_GRASS; // only grass is walkable
}
PlayScene::TileType VillageScene::GetDefaultWalkableTile() const {
    return TILE_GRASS;
}

void VillageScene::Initialize() {
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
    lives = 10;
    money = 0;
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


    dialogueBoxImage = new Engine::Image("play/dialogue.png", halfW - 600, h - 210, 1140, 150);
    dialogueBoxImage->Visible = false;
    UIGroup->AddNewObject(dialogueBoxImage);

    // Rin normal
    rin_normal = new Engine::Image("play/rin normal.png", halfW - 950, h - 480, 720, 480);
    rin_normal->Visible = false;
    AddNewObject(rin_normal);

    // Rin worried
    rin_worry = new Engine::Image("play/rin worried.png", halfW - 950, h - 480, 720, 480);
    rin_worry->Visible = false;
    AddNewObject(rin_worry);

    // Rin close
    rin_close = new Engine::Image("play/rin close.png", halfW - 950, h - 480, 720, 480);
    rin_close->Visible = false;
    AddNewObject(rin_close);

    // Toma happy
    toma_happy = new Engine::Image("play/toma happy.png", halfW - 950, h - 480, 720, 480);
    toma_happy->Visible = false;
    AddNewObject(toma_happy);

    // Toma shock
    toma_shock = new Engine::Image("play/toma shock.png", halfW - 950, h - 480, 720, 480);
    toma_shock->Visible = false;
    AddNewObject(toma_shock);

    // Toma worry
    toma_worry = new Engine::Image("play/toma worry.png", halfW - 950, h - 480, 720, 480);
    toma_worry->Visible = false;
    AddNewObject(toma_worry);


    if (!dialogueLines.empty()) {
        dialogueLabel = new Engine::Label(dialogueLines[0], "To The Point.ttf", 70, startX, startY, 255, 255, 255, 255, 0.0, 0.5);
        UIGroup->AddNewObject(dialogueLabel);
    }


    // Create house
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
        512, 300,
        "npc/toma",
        "intro");
    EffectGroup->AddNewObject(npcTalker);


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
    //bgmId = AudioHelper::PlayBGM("village-explore.mp3");
}
void VillageScene::ReadMap() {
    // load file map.txt jd mapState
    // ngegambar tile floor / dirt ke TileMapGroup
    std::string filename = std::string("Resource/map") + std::to_string(MapId) + ".txt";
    // Read map file.
    char c;
    std::vector<int> mapData;
    std::ifstream fin(filename);
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



void VillageScene::OnKeyDown(int keyCode) {
    //handle shortcut turret Q/W/E/R
// juga logic cheat code masuk disini (bisa spawn plane + 10k)
    std::cout << "Pressed: " << keyCode << std::endl;

    if (keyCode == ALLEGRO_KEY_SPACE && dialogueActive) {
        std::cout << "SPACE PRESSED & dialogueActive is TRUE\n";
        AdvanceDialogue();
        return;
    }
    IScene::OnKeyDown(keyCode);
    if (keyCode == ALLEGRO_KEY_W || keyCode == 84 || keyCode == ALLEGRO_KEY_UP) keyUpDown = true;
    if (keyCode == ALLEGRO_KEY_S || keyCode == 85 || keyCode == ALLEGRO_KEY_DOWN) keyDownDown = true;
    if (keyCode == ALLEGRO_KEY_A || keyCode == 82 || keyCode == ALLEGRO_KEY_LEFT) keyLeftDown = true;
    if (keyCode == ALLEGRO_KEY_D || keyCode == 83 || keyCode == ALLEGRO_KEY_RIGHT) keyRightDown = true;


    if (keyCode == ALLEGRO_KEY_TAB) {
        DebugMode = !DebugMode;
    }
    else {
        keyStrokes.push_back(keyCode);
        if (keyStrokes.size() > code.size())
            keyStrokes.pop_front();

        bool matched = keyStrokes.size() >= code.size() &&
            std::equal(code.begin(), code.end(), std::prev(keyStrokes.end(), code.size()));

        if (matched) {
            AddNewObject(new Plane());
            EarnMoney(10000);
            cheatLabel = new Engine::Label("U DISCOVERED THE CHEAT CODE!", "pirulen.ttf", 48,
                                                640, 300, 255, 0, 0, 255);
            cheatLabel->Anchor = Engine::Point(0.5, 0.5);
            UIGroup->AddNewObject(cheatLabel);
            cheatTimer = 0.0f;         // timer for the text
            cheatActive = true;        // to track it lah peppi
            std::cout << "Cheat Code Matched, adding 10k to money and spawning plane\n";
            keyStrokes.clear();
        }
    }
    if (keyCode == ALLEGRO_KEY_Q) {
        // Hotkey for MachineGunTurret.
        UIBtnClicked(0);
    } else if (keyCode == ALLEGRO_KEY_T) {
        // Hotkey for LaserTurret.
        UIBtnClicked(1);
    } else if (keyCode == ALLEGRO_KEY_E) {
        // Hotkey for FireTurret.
        UIBtnClicked(2);
    } else if (keyCode == ALLEGRO_KEY_R) {
        // Hotkey for RocketTurretw.
        UIBtnClicked(3);
    }
    else if (keyCode >= ALLEGRO_KEY_0 && keyCode <= ALLEGRO_KEY_9) {
        // Hotkey for Speed up.
        SpeedMult = keyCode - ALLEGRO_KEY_0;
    }
}

void VillageScene::OnKeyUp(int keyCode) {
    IScene::OnKeyUp(keyCode);

    if (keyCode == ALLEGRO_KEY_W || keyCode == 84 || keyCode == ALLEGRO_KEY_UP) keyUpDown = false;
    if (keyCode == ALLEGRO_KEY_S || keyCode == 85 || keyCode == ALLEGRO_KEY_DOWN) keyDownDown = false;
    if (keyCode == ALLEGRO_KEY_A || keyCode == 82 || keyCode == ALLEGRO_KEY_LEFT) keyLeftDown = false;
    if (keyCode == ALLEGRO_KEY_D || keyCode == 83 || keyCode == ALLEGRO_KEY_RIGHT) keyRightDown = false;
}

void VillageScene::ShowDialogue(const std::vector<std::string>& lines) {
    auto screenSize = Engine::GameEngine::GetInstance().GetScreenSize();
    int halfW = screenSize.x / 2;
    int startX = halfW - 600;
    int startY = screenSize.y - 140;
    int screenH = Engine::GameEngine::GetInstance().GetScreenSize().y;

    dialogueLines = lines;
    currentDialogueIndex = 0;
    dialogueActive = true;

    // Create dialogue box image if not already created
    if (!dialogueBoxImage) {
        dialogueBoxImage = new Engine::Image("play/dialogue.png", halfW - 600, screenSize.y - 210, 1140, 150);
        UIGroup->AddNewObject(dialogueBoxImage);
    }
    dialogueBoxImage->Visible = true;


    // Create dialogue label if not already created
    if (!dialogueLabel) {
        dialogueLabel = new Engine::Label("", "To The Point.ttf", 70, startX-15, startY, 255, 255, 255, 255);
        dialogueLabel->Anchor = Engine::Point(0.0, 0.5);
        UIGroup->AddNewObject(dialogueLabel);
    }
    dialogueLabel->Visible = true;

    // Set first line of dialogue
    if (!dialogueLines.empty()) {
        std::cout << "Setting initial dialogue line: " << dialogueLines[0] << std::endl;

        dialogueLabel->Text = ""; // Force refresh
        dialogueLabel->Text = dialogueLines[0];
        dialogueLabel->Visible = true;
        dialogueLabel->Position.x = halfW - 450;
        dialogueLabel->Position.y = screenH - 150;

        currentDialogueIndex = 1;
    }

}

void VillageScene::AdvanceDialogue() {
    if (!dialogueActive) return;

    if (currentDialogueIndex < (int)dialogueLines.size()) {
        std::cout << "Advancing to line: " << dialogueLines[currentDialogueIndex] << std::endl;

        // Force refresh text
        if (dialogueLabel)dialogueLabel->Text = "";
        if (dialogueLabel)dialogueLabel->Text = dialogueLines[currentDialogueIndex];
        // Optionally force position and visibility again
        if (dialogueLabel)dialogueLabel->Visible = true;

        if (rin_normal) rin_normal->Visible = false;
        if (rin_worry) rin_worry->Visible = false;
        if (rin_close) rin_close->Visible = false;
        if (toma_happy) toma_happy->Visible = false;
        if (toma_shock) toma_shock->Visible = false;
        if (toma_worry) toma_worry->Visible = false;

        if (currentDialogueIndex == 22) {
            EarnMoney(500); // or any value you want to reward
            std::cout << "[DEBUG] Awarded 500 money for reaching dialogue 22\n";
        }
        // Show expressions on specific lines
        //toma worry
        if (currentDialogueIndex == 4 || currentDialogueIndex == 5 || currentDialogueIndex == 8 || currentDialogueIndex == 9 || currentDialogueIndex == 10 || currentDialogueIndex == 11 || currentDialogueIndex == 12) {
            if (toma_worry) toma_worry->Visible = true;
        } // toma happy
        else if (currentDialogueIndex == 3 || currentDialogueIndex == 6 || currentDialogueIndex == 17 || currentDialogueIndex == 21 || currentDialogueIndex == 23 || currentDialogueIndex == 24 || currentDialogueIndex == 16) {
            if (toma_happy) toma_happy->Visible = true;
        } //toma shock
        else if (currentDialogueIndex == 0 || currentDialogueIndex == 1 || currentDialogueIndex == 2 || currentDialogueIndex == 13 || currentDialogueIndex == 15)
        {
            if (toma_shock) toma_shock->Visible = true;
        }//rin worry
        else if (currentDialogueIndex == 7)
        {
            if (rin_worry) rin_worry->Visible = true;
        }//rin normal
        else if (currentDialogueIndex == 20)
        {
            if (rin_normal) rin_normal->Visible = true;
        }//rin close
        else if (currentDialogueIndex == 14)
        {
            if (rin_close) rin_close->Visible = true;
        }

        currentDialogueIndex++;
    } else {
        std::cout << "End of dialogue.\n";
        if (rin_normal) rin_normal->Visible = false;
        if (rin_worry) rin_worry->Visible = false;
        if (rin_close) rin_close->Visible = false;
        if (toma_happy) toma_happy->Visible = false;
        if (toma_shock) toma_shock->Visible = false;
        if (toma_worry) toma_worry->Visible = false;
        dialogueActive = false;
        if (dialogueBoxImage) dialogueBoxImage->Visible = false;
        if (dialogueLabel) dialogueLabel->Visible = false;
    }
}


void VillageScene::HideDialogue() {
    dialogueActive = false;
    if (dialogueBoxImage) dialogueBoxImage->Visible = false;
    if (dialogueLabel) dialogueLabel->Visible = false;
}

void VillageScene::Update(float deltaTime) {
    PlayScene::Update(deltaTime);
    Transition();
}

void VillageScene::Transition() {
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