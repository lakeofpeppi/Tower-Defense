#include <algorithm>
#include <allegro5/allegro.h>
#include <cmath>
#include <fstream>
#include <functional>
#include <memory>
#include <queue>
#include <string>
#include <vector>
#include "Enemy/PlaneEnemy.hpp"
#include "Enemy/Enemy.hpp"
#include "Enemy/SoldierEnemy.hpp"
#include "Enemy/TankEnemy.hpp"
#include "Enemy/AquaTankEnemy.hpp"
#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "Engine/LOG.hpp"
#include "Engine/Resources.hpp"
#include "PlayScene.hpp"
#include "UI/Component/ImageButton.hpp"
#include "GameData.hpp"
#include <filesystem>
#include "Enemy/OrcEnemy.hpp"
#include "Enemy/JellyFishEnemy.hpp"
#include "Enemy/ScorpionEnemy.hpp"
#include "Enemy/FrogEnemy.hpp"

#include "GameData.hpp"
#include "IntroScene.hpp"
#include "Turret/LaserTurret.hpp"
#include "Turret/FireTurret.hpp"
#include "Turret/MachineGunTurret.hpp"
#include "Turret/RocketTurret.hpp"
#include "Turret/TurretButton.hpp"
#include "Turret/ShovelTool.hpp"
#include "UI/Animation/DirtyEffect.hpp"
#include "UI/Animation/Plane.hpp"
#include "UI/Component/Label.hpp"
#include "Scene/WinScene.hpp"
#include "Character/RinCharacter.hpp"
#include "Helper/House.hpp"
#include "Helper/NPC.hpp"
#include "Helper/Fence.hpp"
#include "Scene/VillageScene.hpp"

// TODO HACKATHON-4 (1/3): Trace how the game handles keyboard input.
// TODO HACKATHON-4 (2/3): Find the cheat code sequence in this file.
// TODO HACKATHON-4 (3/3): When the cheat code is entered, a plane should be spawned and added to the scene.
// TODO HACKATHON-5 (1/4): There's a bug in this file, which crashes the game when you win. Try to find it.
// TODO HACKATHON-5 (2/4): The "LIFE" label are not updated when you lose a life. Try to fix it.

bool PlayScene::DebugMode = false;
const std::vector<Engine::Point> PlayScene::directions = { Engine::Point(-1, 0), Engine::Point(0, -1), Engine::Point(1, 0), Engine::Point(0, 1) };
const int PlayScene::MapWidth = 23, PlayScene::MapHeight = 19;
const int PlayScene::BlockSize = 64;
const float PlayScene::DangerTime = 7.61;
const Engine::Point PlayScene::SpawnGridPoint = Engine::Point(-1, 0);
const Engine::Point PlayScene::EndGridPoint = Engine::Point(MapWidth, MapHeight - 1);
const std::vector<int> PlayScene::code = {
    /*ALLEGRO_KEY_UP, ALLEGRO_KEY_UP, ALLEGRO_KEY_DOWN, ALLEGRO_KEY_DOWN,
    ALLEGRO_KEY_LEFT, ALLEGRO_KEY_RIGHT, ALLEGRO_KEY_LEFT, ALLEGRO_KEY_RIGHT,
    ALLEGRO_KEY_B, ALLEGRO_KEY_A, ALLEGRO_KEY_L_SHIFT, ALLEGRO_KEY_ENTER
    */
    84, 84,     // UP, UP
    85, 85,     // DOWN, DOWN
    82, 83,     // LEFT, RIGHT
    82, 83,     // LEFT, RIGHT
    2, 1,       // B, A
    215, 67     // Left Shift, Enter
};

Engine::Point PlayScene::GetClientSize() {
    return Engine::Point(MapWidth * BlockSize, MapHeight * BlockSize);
}

PlayScene::TileType PlayScene::GetDefaultWalkableTile() const {
    return TILE_GRASS;
}


void PlayScene::Terminate() {
    // dipanggil pas scene selesai, kayak exit stage
    // ini nge-stop bgm dan clean memory
    AudioHelper::StopBGM(bgmId);
    AudioHelper::StopSample(deathBGMInstance);
    deathBGMInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
    IScene::Terminate();
}

void PlayScene::Update(float deltaTime) {
    // ini main loop per frame. all game logic jalan disini tiap tick
    // spawn musuh, update status enemy, win/lose check, preview turret gerak, cheat code timer etc
    // If we use deltaTime directly, then we might have Bullet-through-paper problem.
    // Reference: Bullet-Through-Paper
    if (GameData::poisonStingEquip) {
        double currentTime = al_get_time();
        if (currentTime - poisonStingStartTime >= 3.0) {
            GameData::poisonStingEquip = false;
            poisonStingLabel->Visible = false;
            std::cout << "Poison sting unequipped" << std::endl;
        }
    }

    if (invincible) {
        double currentTime = al_get_time();
        if (currentTime - invincibleStartTime >= 5.0) {
            GameData::isInvincible = false;
            std::cout << "Invincibility ended" << std::endl;
            InvincibleLabel->Visible = false;
        }
    }
    if (null_emotion) {
        double currentTime = al_get_time();
        if (currentTime - null_emotionStartTime >= 10.0) {
            GameData::isNull = false;
            std::cout << "Null emotion ended" << std::endl;
            null_emotionLabel->Visible = false;
        }
    }
    if (SpeedMult == 0)
        deathCountDown = -1;
    else if (deathCountDown != -1)
        SpeedMult = 1;
    std::vector<float> reachEndTimes;
    for (auto &it : EnemyGroup->GetObjects()) {
        reachEndTimes.push_back(dynamic_cast<Enemy *>(it)->reachEndTime);
    }
    std::sort(reachEndTimes.begin(), reachEndTimes.end());
    float newDeathCountDown = -1;
    int danger = GameData::lives;
    for (auto &it : reachEndTimes) {
        if (it <= DangerTime) {
            danger--;
            if (danger <= 0) {
                float pos = DangerTime - it;
                if (it > deathCountDown) {
                    AudioHelper::StopSample(deathBGMInstance);
                    if (SpeedMult != 0)
                        deathBGMInstance = AudioHelper::PlaySample("astronomia.ogg", false, AudioHelper::BGMVolume, pos);
                }
                float alpha = pos / DangerTime;
                alpha = std::max(0, std::min(255, static_cast<int>(alpha * alpha * 255)));
                dangerIndicator->Tint = al_map_rgba(255, 255, 255, alpha);
                newDeathCountDown = it;
                break;
            }
        }
    }
    deathCountDown = newDeathCountDown;
    if (SpeedMult == 0)
        AudioHelper::StopSample(deathBGMInstance);
    if (deathCountDown == -1 && GameData::lives > 0) {
        AudioHelper::StopSample(deathBGMInstance);
        dangerIndicator->Tint.a = 0;
    }
    if (SpeedMult == 0)
        deathCountDown = -1;


    RinCharacter* rin = nullptr;
for (auto& obj : EffectGroup->GetObjects()) {
    rin = dynamic_cast<RinCharacter*>(obj);
    if (rin) break;
}

for (int i = 0; i < SpeedMult; i++) {
    IScene::Update(deltaTime);

    if (rin) {
        for (auto& obj : EffectGroup->GetObjects()) {
            float dx = obj->Position.x - rin->Position.x;
            float dy = obj->Position.y - rin->Position.y;
            float distance = std::sqrt(dx * dx + dy * dy);

            if (auto* frog = dynamic_cast<FrogEnemy*>(obj)) {
                if (distance < 128) {
                    frog->OnTouch();
                }
            } else if (distance < 64) {
                if (auto* npc = dynamic_cast<NPC*>(obj)) {
                    npc->OnTouch();
                } else if (auto* orc = dynamic_cast<OrcEnemy*>(obj)) {
                    orc->OnTouch();
                } else if (auto* scorpion = dynamic_cast<ScorpionEnemy*>(obj)) {
                    scorpion->OnTouch();
                } else if (auto* jelly = dynamic_cast<JellyFishEnemy*>(obj)) {
                    jelly->OnTouch();
                } else if (auto* house = dynamic_cast<House*>(obj)) {
                    house->OnTouch();
                }
            }
        }
    }

    ticks += deltaTime;
    if (enemyWaveData.empty()) {
        if (EnemyGroup->GetObjects().empty()) {
            continue;
        }
        continue;
    }
    auto current = enemyWaveData.front();
    if (ticks < current.second) continue;
    ticks -= current.second;
    enemyWaveData.pop_front();

    const Engine::Point SpawnCoordinate = Engine::Point(SpawnGridPoint.x * BlockSize + BlockSize / 2, SpawnGridPoint.y * BlockSize + BlockSize / 2);
    Enemy *enemy;
    switch (current.first) {
        case 1:
            enemy = new SoldierEnemy(SpawnCoordinate.x, SpawnCoordinate.y);
            break;
        case 2:
            enemy = new PlaneEnemy(SpawnCoordinate.x, SpawnCoordinate.y);
            break;
        case 3:
            enemy = new TankEnemy(SpawnCoordinate.x, SpawnCoordinate.y);
            break;
        case 4:
            enemy = new AquaTankEnemy(SpawnCoordinate.x, SpawnCoordinate.y);
            break;
        default:
            continue;
    }
    EnemyGroup->AddNewObject(enemy);
    enemy->UpdatePath(mapDistance);
    enemy->Update(ticks);
}

    if (preview) {
        preview->Position = Engine::GameEngine::GetInstance().GetMousePosition();
        preview->Update(deltaTime);
    }
    if (UILives) {
        ALLEGRO_COLOR alertRed = al_map_rgb(255, 40, 40);
        ALLEGRO_COLOR current = UILives->Color;
        if (current.r == alertRed.r && current.g == alertRed.g && current.b == alertRed.b) {
            UILives->Color = al_map_rgb(255, 255, 255);
        }
    }
    if (cheatActive) {
        cheatTimer += deltaTime;
        if (cheatTimer >= 5.0f && cheatLabel) {
            UIGroup->RemoveObject(cheatLabel->GetObjectIterator());
            cheatLabel = nullptr;
            cheatActive = false;
        }
    }
    if (refundActive) {
        refundTimer += deltaTime;
        if (refundTimer >= 2.0f && refundLabel) {
            UIGroup->RemoveObject(refundLabel->GetObjectIterator());
            refundLabel = nullptr;
            refundActive = false;
        }
    }
}

void PlayScene::Draw() const {

    IScene::Draw();
    if (DebugMode) {
        for (int i = 0; i < MapHeight; i++) {
            for (int j = 0; j < MapWidth; j++) {
                if (mapDistance[i][j] != -1) {
                    Engine::Label label(std::to_string(mapDistance[i][j]), "pirulen.ttf", 32, (j + 0.5) * BlockSize, (i + 0.5) * BlockSize);
                    label.Anchor = Engine::Point(0.5, 0.5);
                    label.Draw();
                    std::cout << "TileMapGroup drawing done.\n";
                }
            }
        }
    }
}

void PlayScene::OnMouseDown(int button, int mx, int my) {
    if ((button & 1) && !imgTarget->Visible && preview) {
        UIGroup->RemoveObject(preview->GetObjectIterator());
        preview = nullptr;
    }
    IScene::OnMouseDown(button, mx, my);
}
void PlayScene::OnMouseMove(int mx, int my) {
    IScene::OnMouseMove(mx, my);
    const int x = mx / BlockSize;
    const int y = my / BlockSize;
    if (!preview || x < 0 || x >= MapWidth || y < 0 || y >= MapHeight) {
        imgTarget->Visible = false;
        return;
    }
    imgTarget->Visible = true;
    imgTarget->Position.x = x * BlockSize;
    imgTarget->Position.y = y * BlockSize;
}

void PlayScene::OnMouseUp(int button, int mx, int my) {
    IScene::OnMouseUp(button, mx, my);
    if (!imgTarget->Visible) return;

    const int x = mx / BlockSize;
    const int y = my / BlockSize;

    if (!(button & 1) || !preview) return;
    ShovelTool* shovel = dynamic_cast<ShovelTool*>(preview);
    if (shovel) {
        Turret* target = nullptr;
        for (auto& obj : TowerGroup->GetObjects()) {
            Turret* turret = dynamic_cast<Turret*>(obj);
            if (!turret || dynamic_cast<ShovelTool*>(turret)) continue;
            int tx = floor(turret->Position.x / BlockSize);
            int ty = floor(turret->Position.y / BlockSize);
            if (tx == x && ty == y) {
                target = turret;
                break;
            }
        }
        if (target) {
            TowerGroup->RemoveObject(target->GetObjectIterator());
            mapState[y][x] = GetDefaultWalkableTile();
            int refund = 25;
            EarnMoney(refund);
            refundLabel = new Engine::Label("+$" + std::to_string(refund), "pirulen.ttf", 24, mx, my, 0, 255, 0);
            refundLabel->Anchor = Engine::Point(0.5, 0.5);
            UIGroup->AddNewObject(refundLabel);
            refundTimer = 0.0f;
            refundActive = true;
            std::cout << "[DEBUG] Deleting turret at (" << x << "," << y << "), refund = $" << target->GetPrice() << std::endl;
            keyStrokes.clear();
            Engine::Sprite* sprite;
            GroundEffectGroup->AddNewObject(sprite = new DirtyEffect("play/target-invalid.png", 1,
                x * BlockSize + BlockSize / 2,
                y * BlockSize + BlockSize / 2));
            sprite->Rotation = 0;
        }


        UIGroup->RemoveObject(preview->GetObjectIterator());
        preview = nullptr;
        return;
    }
    if (mapState[y][x] != TILE_OCCUPIED) {
        if (!CheckSpaceValid(x, y, preview->GetTurretID())) {
            GroundEffectGroup->AddNewObject(
                new DirtyEffect("play/target-invalid.png", 1,
                    x * BlockSize + BlockSize / 2,
                    y * BlockSize + BlockSize / 2)
            );
            return;
        }
        if (preview->GetTurretID() == 5 && x == 14 && y == 14) {
            GameData::isRose = true;
            std::cout << "Rose activated!" << std::endl;

            AudioHelper::PlaySample("collect.mp3");
            Engine::ImageButton* btn = new TurretButton("play/floor.png", "play/dirt.png",
                       Engine::Sprite("play/rose glass.png", 1714, 776, 64, 64, 0, 0),
                       Engine::Sprite("play/rose glass.png", 1714, 776, 64, 64, 0, 0), 1714, 776, ShovelTool::Price);
            btn->SetOnClickCallback(std::bind(&PlayScene::UIBtnClicked, this, 9));
            UIGroup->AddNewControlObject(btn);
        }

    }

        EarnMoney(-preview->GetPrice());
        preview->GetObjectIterator()->first = false;
        UIGroup->RemoveObject(preview->GetObjectIterator());
        preview->Position.x = x * BlockSize + BlockSize / 2;
        preview->Position.y = y * BlockSize + BlockSize / 2;
        preview->Enabled = true;
        preview->Preview = false;
        preview->Tint = al_map_rgba(255, 255, 255, 255);
        TowerGroup->AddNewObject(preview);
        preview->Update(0);
        preview = nullptr;

        mapState[y][x] = TILE_OCCUPIED;
        OnMouseMove(mx, my);
    }


void PlayScene::OnKeyDown(int keyCode) {
    std::cout << "Pressed: " << keyCode << std::endl;
    IScene::OnKeyDown(keyCode);
    if (keyCode == ALLEGRO_KEY_W || keyCode == 84 || keyCode == ALLEGRO_KEY_UP)keyUpDown = true;
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
            cheatTimer = 0.0f;
            cheatActive = true;
            std::cout << "Cheat Code Matched, adding 10k to money and spawning plane\n";
            keyStrokes.clear();
        }
    }
    // if (keyCode == ALLEGRO_KEY_Q) {
    //     // Hotkey for MachineGunTurret.
    //     UIBtnClicked(0);
    // } else if (keyCode == ALLEGRO_KEY_W) {
    //     // Hotkey for LaserTurret.
    //     UIBtnClicked(1);
    // } else if (keyCode == ALLEGRO_KEY_E) {
    //     // Hotkey for FireTurret.
    //     UIBtnClicked(2);
    // } else if (keyCode == ALLEGRO_KEY_R) {
    //     // Hotkey for RocketTurretw.
    //     UIBtnClicked(3);
    // }
    // else if (keyCode >= ALLEGRO_KEY_0 && keyCode <= ALLEGRO_KEY_9) {
    //     // Hotkey for Speed up.
    //     SpeedMult = keyCode - ALLEGRO_KEY_0;
    // }
}

void PlayScene::OnKeyUp(int keyCode) {
    IScene::OnKeyUp(keyCode);

    if (keyCode == ALLEGRO_KEY_W || keyCode == 84 || keyCode == ALLEGRO_KEY_UP) keyUpDown = false;
    if (keyCode == ALLEGRO_KEY_S || keyCode == 85 || keyCode == ALLEGRO_KEY_DOWN) keyDownDown = false;
    if (keyCode == ALLEGRO_KEY_A || keyCode == 82 || keyCode == ALLEGRO_KEY_LEFT) keyLeftDown = false;
    if (keyCode == ALLEGRO_KEY_D || keyCode == 83 || keyCode == ALLEGRO_KEY_RIGHT) keyRightDown = false;
}

void PlayScene::Hit() {
    // ini dipanggil kalo enemy tembus base. ngurangin nyawa dan update UI life
    GameData::lives--;
    if (UILives) {
        UILives->Text = std::string("Life ") + std::to_string(GameData::lives);
        UILives->Color = al_map_rgb(255, 50, 50);
    }
    if (GameData::lives <= 0) {
        Engine::GameEngine::GetInstance().ChangeScene("lose");
    }
}



int PlayScene::GetMoney() const {
    // getter buat current money. i can call dari luar
    return GameData::money;
}

void PlayScene::EarnMoney(int money) {
    // update label UI money bisa nambah bisa ngurang
    GameData::money += money;
    UIMoney->Text = std::string("COINS: ") + std::to_string(GameData::money);
}


void PlayScene::GainKnowledge()
{
    GameData::knowledge = 100;
    UIKnowledge->Text = std::string("Knowledge -- ") + std::to_string(GameData::knowledge);
}

void PlayScene::LoseSpeed()
{
    GameData::speed -= 20;
    UIKnowledge->Text = std::string("Speed -- ") + std::to_string(GameData::speed);
}

void PlayScene::LoseStrength()
{
    GameData::strength -= 20;
    UIKnowledge->Text = std::string("Strength -- ") + std::to_string(GameData::strength);
}

void PlayScene::ReadEnemyWave() {
    // load file enemyX.txt buat wave musuh
    // tiap baris disimpan sebagai pair (tipe musuh, interval muncul)
    std::string filename = std::string("Resource/enemy") + std::to_string(MapId) + ".txt";
    // Read enemy file.
    float type, wait, repeat;
    enemyWaveData.clear();
    std::ifstream fin(filename);
    while (fin >> type && fin >> wait && fin >> repeat) {
        for (int i = 0; i < repeat; i++)
            enemyWaveData.emplace_back(type, wait);
    }
    fin.close();
}

void PlayScene::ConstructUI() {
    // setup tombol-tombol turret, text UI (stage, money, lives)
    // Background
    InvincibleLabel = new Engine::Label("INVINCIBLE!", "pirulen.ttf", 28,
     736, 30, 64, 224, 208, 255, 0.5f, 0.5f);
    InvincibleLabel->Visible = false;
    UIGroup->AddNewObject(InvincibleLabel);

    null_emotionLabel = new Engine::Label("NULL EMOTIONS!", "pirulen.ttf", 28,
        736, 60, 128, 0, 128, 255, 0.5f, 0.5f);
    null_emotionLabel->Visible = false;
    UIGroup->AddNewObject(null_emotionLabel);

    poisonStingLabel = new Engine::Label("POISON STING EQUIPPED!", "pirulen.ttf", 28,
        736, 90, 0, 255, 0, 255, 0.5f, 0.5f);
    poisonStingLabel->Visible = false;
    UIGroup->AddNewObject(poisonStingLabel);



    UIGroup->AddNewObject(new Engine::Image("play/wooden_forest.png", 1472, 0, 320, 1216));
    // Text
    UIGroup->AddNewObject(UILives = new Engine::Label(std::string("HP: ") + std::to_string(GameData::lives), "pirulen.ttf", 32, 1500, 300));
    UIGroup->AddNewObject(UIMoney = new Engine::Label(std::string("COINS: ") + std::to_string(GameData::money), "pirulen.ttf", 32, 1500, 350));
    //UIGroup->AddNewObject(UILives = new Engine::Label(std::string("STAGE: ") + std::to_string(MapId), "pirulen.ttf", 24, 1486, 400));
    UIGroup->AddNewObject(UIKnowledge = new Engine::Label(std::string("Knowledge -- ") + std::to_string(GameData::knowledge), "pirulen.ttf", 24, 1486, 450));
    UIGroup->AddNewObject(UISpeed = new Engine::Label(std::string("Speed -- ") + std::to_string(GameData::speed), "pirulen.ttf", 24, 1486, 500));
    UIGroup->AddNewObject(UIStrength = new Engine::Label(std::string("Strength -- ") + std::to_string(GameData::strength), "pirulen.ttf", 24, 1486, 550));
    TurretButton *btn;
    // Button 1
    btn = new TurretButton("play/floor.png", "play/dirt.png",
                           Engine::Sprite("play/apple.png", 1486, 700, 0, 0, 0, 0),
                           Engine::Sprite("play/apple.png", 1486, 700, 0, 0, 0, 0), 1486, 700, LaserTurret::Price);
    // Reference: Class Member Function Pointer and std::bind.
    btn->SetOnClickCallback(std::bind(&PlayScene::UIBtnClicked, this, 0));
    UIGroup->AddNewControlObject(btn);

    // Button 2
    btn = new TurretButton("play/floor.png", "play/dirt.png",
                           Engine::Sprite("play/orange.png", 1562, 705, 0, 0, 0, 0),
                           Engine::Sprite("play/orange.png", 1562, 705, 0, 0, 0, 0), 1562, 700, LaserTurret::Price);
    btn->SetOnClickCallback(std::bind(&PlayScene::UIBtnClicked, this, 1));
    UIGroup->AddNewControlObject(btn);

    // fire turret
    btn = new TurretButton("play/floor.png", "play/dirt.png",
                       Engine::Sprite("play/null.png", 1636, 696, 0, 0, 0, 0),
                       Engine::Sprite("play/null.png", 1636, 696, 0, 0, 0, 0), 1636, 700, FireTurret::Price);
    btn->SetOnClickCallback(std::bind(&PlayScene::UIBtnClicked, this, 2)); // ID = 2
    UIGroup->AddNewControlObject(btn);

    //rocket turret
    btn = new TurretButton("play/floor.png", "play/dirt.png",
                       Engine::Sprite("play/invinsible.png", 1714, 696, 0, 0, 0, 0),
                       Engine::Sprite("play/invinsible.png", 1714, 696, 0, 0, 0, 0), 1714, 700, FireTurret::Price);
    btn->SetOnClickCallback(std::bind(&PlayScene::UIBtnClicked, this, 3));
    UIGroup->AddNewControlObject(btn);


    if (GameData::scorpionHP == 0)
    {
        btn = new TurretButton("play/floor.png", "play/dirt.png",
                       Engine::Sprite("play/poison sting.png", 1636, 776, 0, 0, 0, 0),
                       Engine::Sprite("play/poison sting.png", 1636, 776, 0, 0, 0, 0), 1636, 776, ShovelTool::Price);
        btn->SetOnClickCallback(std::bind(&PlayScene::UIBtnClicked, this, 4));
        UIGroup->AddNewControlObject(btn);
    }


    if (GameData::orcHP == 0) {
        btn = new TurretButton("play/floor.png", "play/dirt.png",
                               Engine::Sprite("play/bone.png", 1562, 776, 0, 0, 0, 0),
                               Engine::Sprite("play/bone.png", 1562, 776, 0, 0, 0, 0),
                               1562, 776, MachineGunTurret::Price);
        btn->SetOnClickCallback(std::bind(&PlayScene::UIBtnClicked, this, 5));
        UIGroup->AddNewControlObject(btn);
    }




    btn = new TurretButton("play/floor.png", "play/dirt.png",
                       Engine::Sprite("play/trap.png", 1486, 776, 0, 0, 0, 0),
                       Engine::Sprite("play/trap.png", 1486, 776 , 0, 0, 0, 0), 1486, 776, RocketTurret::Price);
    btn->SetOnClickCallback(std::bind(&PlayScene::UIBtnClicked, this, 6));
    UIGroup->AddNewControlObject(btn);



    blackScreen = new Engine::Image("play/floor.png", 400, 200, 672, 816);
    blackScreen->Visible = false;
    UIGroup->AddNewObject(blackScreen);

    //MENU
    Engine::ImageButton* menuButton = new Engine::ImageButton(
    "stage-select/dirt.png",
    "stage-select/floor.png",
    1486, 900,
    292, 60
     );
    menuButton->SetOnClickCallback(std::bind(&PlayScene::MenuOnClick, this));
    UIGroup->AddNewControlObject(menuButton);

    UIGroup->AddNewObject(new Engine::Label("MENU", "pirulen.ttf", 32,
     1632, 930, 255, 255, 255, 255, 0.5f, 0.5f));

    Engine::ImageButton* mapButton = new Engine::ImageButton(
        "stage-select/dirt.png", "stage-select/floor.png",
        1486, 970, 292, 60
    );
    mapButton->SetOnClickCallback(std::bind(&PlayScene::MapOnClick, this));
    UIGroup->AddNewControlObject(mapButton);

    UIGroup->AddNewObject(new Engine::Label("MAP", "pirulen.ttf", 32,
        1632, 1000, 255, 255, 255, 255, 0.5f, 0.5f));



    saveBtn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", 520, 300, 400, 60);
    saveBtn->Visible = false;
    saveBtn->SetOnClickCallback(std::bind(&PlayScene::SaveProgressOnClick, this));
    UIGroup->AddNewControlObject(saveBtn);

    saveLabel = new Engine::Label("Save Progress", "pirulen.ttf", 28, 720, 330, 255, 255, 255, 255, 0.5f, 0.5f);
    saveLabel->Visible = false;
    UIGroup->AddNewObject(saveLabel);

    titleBtn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", 520, 400, 400, 60);
    titleBtn->Visible = false;
    titleBtn->SetOnClickCallback(std::bind(&PlayScene::ReturnToTitleOnClick, this));
    UIGroup->AddNewControlObject(titleBtn);

    titleLabel = new Engine::Label("Return to Title", "pirulen.ttf", 28, 720, 430, 255, 255, 255, 255, 0.5f, 0.5f);
    titleLabel->Visible = false;
    UIGroup->AddNewObject(titleLabel);

    settingsBtn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", 520, 500, 400, 60);
    settingsBtn->Visible = false;
    settingsBtn->SetOnClickCallback(std::bind(&PlayScene::SettingsOnClick, this));
    UIGroup->AddNewControlObject(settingsBtn);

    settingsLabel = new Engine::Label("Settings", "pirulen.ttf", 28, 720, 530, 255, 255, 255, 255, 0.5f, 0.5f);
    settingsLabel->Visible = false;
    UIGroup->AddNewObject(settingsLabel);

    backBtn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", 420, 920, 160, 50);
    backBtn->Visible = false;
    backBtn->SetOnClickCallback(std::bind(&PlayScene::BackOnClick, this));
    UIGroup->AddNewControlObject(backBtn);

    backLabel = new Engine::Label("Back", "pirulen.ttf", 24, 500, 945, 255, 255, 255, 255, 0.5f, 0.5f);
    backLabel->Visible = false;
    UIGroup->AddNewObject(backLabel);


    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int shift = 135 + 25;
    dangerIndicator = new Engine::Sprite("play/benjamin.png", w - shift, h - shift);
    dangerIndicator->Tint.a = 0;
    UIGroup->AddNewObject(dangerIndicator);


    rin_prof = new Engine::Image("play/rin prof.png", 1452, 40, 360, 240);
    rin_prof->Visible = true;
    AddNewObject(rin_prof);
}



void PlayScene::UIBtnClicked(int id) {
    if (preview)
        UIGroup->RemoveObject(preview->GetObjectIterator());
    if ((id == 0 || id == 1) && GameData::money >= 50)
    {
        const int hpCost = 50;
        const int hpGain = 20;
        if (GameData::money >= hpCost) {
            GameData::money -= hpCost;
            std::cout << "lose money"<< std::endl;
            GameData::lives += hpGain;
            std::cout << "gain hp"<< std::endl;
            UIMoney->Text = "COINS: " + std::to_string(GameData::money);
            UILives->Text = "HP: " + std::to_string(GameData::lives);
            AudioHelper::PlaySample("bite.mp3");
        }
        return;
    }
    //buat null emotions
    else if (id == 2 && GameData::money >= 100)
    {
        const int Cost = 100;
        if (GameData::money >= Cost) {
            GameData::money -= Cost;
            std::cout << "lose money"<< std::endl;
            UIMoney->Text = "COINS: " + std::to_string(GameData::money);

            GameData::isNull= true;
            null_emotionStartTime = al_get_time();
            null_emotionLabel->Visible = true;
            AudioHelper::PlaySample("collect.mp3");
        }
        return;
    }
    //buat invincible
    else if (id == 3 && GameData::money >= 100)
    {
        const int Cost = 100;
        if (GameData::money >= Cost) {
            GameData::money -= Cost;
            std::cout << "lose money"<< std::endl;

            UIMoney->Text = "COINS: " + std::to_string(GameData::money);

            GameData::isInvincible = true;
            invincibleStartTime = al_get_time();
            InvincibleLabel->Visible = true;
            AudioHelper::PlaySample("collect.mp3");

        }
        return;
    }else if (id == 6 && GameData::money >= 200)
    {
        AudioHelper::PlaySample("press.mp3");
        preview = new RocketTurret(0, 0);
    }
    else if (id == 5)
    {
        AudioHelper::PlaySample("press.mp3");
        preview = new MachineGunTurret(0, 0);
    }else if (id == 4) {
        double now = al_get_time();
        if (!GameData::poisonStingEquip) {
            GameData::poisonStingEquip = true;
            poisonStingStartTime = now;
            poisonStingLabel->Text = "POISON STING EQUIPPED!";
            poisonStingLabel->Visible = true;
            AudioHelper::PlaySample("press.mp3");
            std::cout << "Poison sting equipped" << std::endl;
        } else {
            GameData::poisonStingEquip = false;
            poisonStingLabel->Text = "POISON STING UNEQUIPPED!";
            poisonStingStartTime = now;
            poisonStingLabel->Visible = true;
            AudioHelper::PlaySample("press.mp3");
            std::cout << "Poison sting unequipped (manual)" << std::endl;
        }
    }else if (id == 9)
    {
        Engine::GameEngine::GetInstance().ChangeScene("memory");
    };

    if (!preview)
        return;
    preview->Position = Engine::GameEngine::GetInstance().GetMousePosition();
    preview->Tint = al_map_rgba(255, 255, 255, 200);
    preview->Enabled = false;
    preview->Preview = true;
    UIGroup->AddNewObject(preview);
    OnMouseMove(Engine::GameEngine::GetInstance().GetMousePosition().x, Engine::GameEngine::GetInstance().GetMousePosition().y);
}

bool PlayScene::CheckSpaceValid(int x, int y, int turretID /* = -1 */) {
    if (x < 0 || x >= MapWidth || y < 0 || y >= MapHeight)
        return false;


    if (turretID == 5) {
        if (mapState[y][x] != TILE_DIG)
            return false;
    }

    auto map00 = mapState[y][x];
    mapState[y][x] = TILE_OCCUPIED;
    std::vector<std::vector<int>> map = CalculateBFSDistance();
    mapState[y][x] = map00;

    if (map[0][0] == -1)
        return false;

    for (auto& it : EnemyGroup->GetObjects()) {
        Engine::Point pnt;
        pnt.x = static_cast<int>(std::clamp(it->Position.x / BlockSize, 0.0f, static_cast<float>(MapWidth - 1)));
        pnt.y = static_cast<int>(std::clamp(it->Position.y / BlockSize, 0.0f, static_cast<float>(MapHeight - 1)));

        if (map[pnt.y][pnt.x] == -1)
            return false;
    }

    mapState[y][x] = TILE_OCCUPIED;
    mapDistance = map;

    for (auto& it : EnemyGroup->GetObjects())
        dynamic_cast<Enemy*>(it)->UpdatePath(mapDistance);

    return true;
}


std::vector<std::vector<int>> PlayScene::CalculateBFSDistance() {
    std::vector<std::vector<int>> map(MapHeight, std::vector<int>(std::vector<int>(MapWidth, -1)));
    std::queue<Engine::Point> que;
    if (mapState[MapHeight - 1][MapWidth - 1] != GetDefaultWalkableTile())
        return map;
    que.push(Engine::Point(MapWidth - 1, MapHeight - 1));
    map[MapHeight - 1][MapWidth - 1] = 0;
    while (!que.empty()) {
        Engine::Point p = que.front();
        que.pop();
        // TODO PROJECT-1 (1/1): Implement a BFS starting from the most right-bottom block in the map.
        //               For each step you should assign the corresponding distance to the most right-bottom block.
        //               mapState[y][x] is TILE_DIRT if it is empty.

        for (const auto& offset : directions) {

            int next_x = p.x + offset.x;
            int next_y = p.y + offset.y;
            if (next_x < 0 || next_x >= MapWidth || next_y < 0 || next_y >= MapHeight)
                continue;
            if (mapState[next_y][next_x] != GetDefaultWalkableTile() || map[next_y][next_x] != -1)
                continue;
            que.push(Engine::Point(next_x, next_y));
        }
    }
    return map;
}
void PlayScene::MenuOnClick() {
    std::cout << "[DEBUG] Menu button clicked\n";

    AudioHelper::PlaySample("press.mp3");
    if (saveBtn)saveBtn->Visible = true;
    if (saveLabel)saveLabel->Visible = true;
    if (titleBtn)titleBtn->Visible = true;
    if (titleBtn)titleLabel->Visible = true;
    if (settingsBtn)settingsBtn->Visible = true;
    if (settingsLabel)settingsLabel->Visible = true;
    if (backBtn)backBtn->Visible = true;
    if (backLabel)backLabel->Visible = true;
    if (blackScreen) blackScreen->Visible = true;
}

void PlayScene::SaveProgressOnClick() {
    std::ofstream fout("Resource/gamedata.txt");
    AudioHelper::PlaySample("press.mp3");
    if (!fout.is_open()) {
        std::cerr << "Error opening gamedata.txt for writing!\n";
        return;
    }

    fout << GameData::lives     << ' '
         << GameData::money     << ' '
         << GameData::knowledge << ' '
         << GameData::strength  << ' '
         << GameData::speed     << ' '
         << "\n";

    std::cout << "[Saved] Lives: " << GameData::lives
              << ", Money: " << GameData::money
              << ", Knowledge: " << GameData::knowledge
              << ", Strength: " << GameData::strength
              << ", Speed: " << GameData::speed << "\n";
}

void PlayScene::LoadProgress() {
    std::ifstream fin("Resource/gamedata.txt");
    if (!fin.is_open()) {
        std::cerr << "[ERROR] Could not open gamedata.txt!\n";
        return;
    }

    fin >> GameData::lives
    >> GameData::money
    >> GameData::knowledge
    >> GameData::strength
    >> GameData::speed;
    std::cout
              << " Lives=" << GameData::lives
              << " Money=" << GameData::money
              << " Know=" << GameData::knowledge << "\n";

}



void PlayScene::ReturnToTitleOnClick() {
    AudioHelper::PlaySample("press.mp3");
    Engine::GameEngine::GetInstance().ChangeScene("start");
}

void PlayScene::SettingsOnClick() {
    AudioHelper::PlaySample("press.mp3");
    // TODO: Implement or transition to SettingsScene
    std::cout << "Opening settings...\n";
    Engine::GameEngine::GetInstance().ChangeScene("settings");
}
void PlayScene::MapOnClick() {
    AudioHelper::PlaySample("press.mp3");
    if (mapVisible) return;
    std::string mapImagePath = GetMapImagePath();

    int mapX = 300;
    int mapY = 100;
    int mapW = 1024;
    int mapH = 1024;

    mapOverlay = new Engine::Image(mapImagePath, mapX, mapY, mapW, mapH);
    UIGroup->AddNewObject(mapOverlay);

    int btnW = 120;
    int btnH = 50;


    int btnX = mapX + 20;
    int btnY = mapY + mapH - btnH - 20;


    closeMapBtn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", btnX, btnY, btnW, btnH);
    closeMapBtn->SetOnClickCallback(std::bind(&PlayScene::CloseMap, this));
    closeMapBtn->Visible = true;
    UIGroup->AddNewControlObject(closeMapBtn);


    closeMapLabel = new Engine::Label("Close", "pirulen.ttf", 20,
        btnX + btnW / 2, btnY + btnH / 2,
        255, 255, 255, 255, 0.5f, 0.5f);
    closeMapLabel->Visible = true;
    UIGroup->AddNewObject(closeMapLabel);

    mapVisible = true;
}


std::string PlayScene::GetMapImagePath() const {

    return "play/map_default.png";
}
void PlayScene::CloseMap() {
    AudioHelper::PlaySample("press.mp3");
    if (!mapVisible) return;

    if (mapOverlay)
        mapOverlay->Visible = false;

    if (closeMapBtn)
        closeMapBtn->Visible = false;

    if (closeMapLabel)
        closeMapLabel->Visible = false;

    mapVisible = false;
}


void PlayScene::BackOnClick() {
    AudioHelper::PlaySample("press.mp3");
    if (blackScreen)blackScreen->Visible = false;
    if (saveBtn)saveBtn->Visible = false;
    if (saveLabel)saveLabel->Visible = false;
    if (titleBtn)titleBtn->Visible = false;
    if (titleLabel)titleLabel->Visible = false;
    if (settingsBtn)settingsBtn->Visible = false;
    if (settingsLabel)settingsLabel->Visible = false;
    if (backBtn)backBtn->Visible = false;
    if (backLabel)backLabel->Visible = false;
}
