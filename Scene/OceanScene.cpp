// //
// // Created by Angelie Melisa on 2025/6/11.
// //
// #include <algorithm>
// #include <allegro5/allegro.h>
// #include <cmath>
// #include <fstream>
// #include <functional>
// #include <memory>
// #include <queue>
// #include <string>
// #include <vector>
// #include "Enemy/PlaneEnemy.hpp"
// #include "Enemy/Enemy.hpp"
// #include "Enemy/SoldierEnemy.hpp"
// #include "Enemy/TankEnemy.hpp"
// #include "Enemy/AquaTankEnemy.hpp"
// #include "Engine/AudioHelper.hpp"
// #include "Engine/GameEngine.hpp"
// #include "Engine/Group.hpp"
// #include "Engine/LOG.hpp"
// #include "Engine/Resources.hpp"
// #include "OceanScene.hpp"
// #include "IntroScene.hpp"
// #include "Turret/LaserTurret.hpp"
// #include "Turret/FireTurret.hpp"
// #include "Turret/MachineGunTurret.hpp"
// #include "Turret/RocketTurret.hpp"
// #include "Turret/TurretButton.hpp"
// #include "Turret/ShovelTool.hpp"
// #include "UI/Animation/DirtyEffect.hpp"
// #include "UI/Animation/Plane.hpp"
// #include "UI/Component/Label.hpp"
// #include "Scene/WinScene.hpp"
// #include "Character/RinCharacter.hpp"
// #include "Helper/House.hpp"
// #include "Helper/NPC.hpp"
//
// // TODO HACKATHON-4 (1/3): Trace how the game handles keyboard input.
// // TODO HACKATHON-4 (2/3): Find the cheat code sequence in this file.
// // TODO HACKATHON-4 (3/3): When the cheat code is entered, a plane should be spawned and added to the scene.
// // TODO HACKATHON-5 (1/4): There's a bug in this file, which crashes the game when you win. Try to find it.
// // TODO HACKATHON-5 (2/4): The "LIFE" label are not updated when you lose a life. Try to fix it.
//
// bool OceanScene::DebugMode = false;
// const std::vector<Engine::Point> OceanScene::directions = { Engine::Point(-1, 0), Engine::Point(0, -1), Engine::Point(1, 0), Engine::Point(0, 1) };
// const int OceanScene::MapWidth = 23, OceanScene::MapHeight = 19;
// const int OceanScene::BlockSize = 64;
// const float OceanScene::DangerTime = 7.61;
// const Engine::Point OceanScene::SpawnGridPoint = Engine::Point(-1, 0);
// const Engine::Point OceanScene::EndGridPoint = Engine::Point(MapWidth, MapHeight - 1);
// const std::vector<int> OceanScene::code = {
//     /*ALLEGRO_KEY_UP, ALLEGRO_KEY_UP, ALLEGRO_KEY_DOWN, ALLEGRO_KEY_DOWN,
//     ALLEGRO_KEY_LEFT, ALLEGRO_KEY_RIGHT, ALLEGRO_KEY_LEFT, ALLEGRO_KEY_RIGHT,
//     ALLEGRO_KEY_B, ALLEGRO_KEY_A, ALLEGRO_KEY_L_SHIFT, ALLEGRO_KEY_ENTER
//     */
//     84, 84,     // UP, UP
//     85, 85,     // DOWN, DOWN
//     82, 83,     // LEFT, RIGHT
//     82, 83,     // LEFT, RIGHT
//     2, 1,       // B, A
//     215, 67     // Left Shift, Enter
// };
//
// Engine::Point OceanScene::GetClientSize() {
//     return Engine::Point(MapWidth * BlockSize, MapHeight * BlockSize);
// }
//
// void OceanScene::Initialize() {
//    // buat initialize stage pas masuk level
//     // ini kayak setup awal: load map, load musuh, set UI,sm start bgms
//     mapState.clear();
//     keyStrokes.clear();
//     ticks = 0;
//     deathCountDown = -1;
//     lives = 10;
//     money = 150;
//     SpeedMult = 1;
//     // Add groups from bottom to top.
//     AddNewObject(TileMapGroup = new Group());
//     AddNewObject(GroundEffectGroup = new Group());
//     AddNewObject(DebugIndicatorGroup = new Group());
//     AddNewObject(TowerGroup = new Group());
//     AddNewObject(EnemyGroup = new Group());
//     AddNewObject(BulletGroup = new Group());
//     AddNewObject(EffectGroup = new Group());
//     // Should support buttons.
//     AddNewControlObject(UIGroup = new Group());
//
//
//     // Create house
//     auto* Inventory = new House(
//         1184, 928,
//         "play/house_inventory.png",
//         "intro");
//     EffectGroup->AddNewObject(Inventory);
//
//     auto* Book = new House(
//         288, 512,  // Example position
//         "play/house_book.png",  // Image of the house
//         "intro"         // The scene it should go to when touched
//     );
//     EffectGroup->AddNewObject(Book);
//
//     auto* npcTalker = new NPC(
//         512, 928,
//         "npc/npc_idle",
//         "intro");
//     EffectGroup->AddNewObject(npcTalker);
//
//
//     auto* rin = new RinCharacter(OceanScene::BlockSize / 2, OceanScene::BlockSize / 2);
//     rin->SetPlayScene(this);
//     EffectGroup->AddNewObject(rin);
//
//
//     ReadMap();
//     ReadEnemyWave();
//     mapDistance = CalculateBFSDistance();
//     ConstructUI();
//     imgTarget = new Engine::Image("play/target.png", 0, 0);
//     imgTarget->Visible = false;
//     preview = nullptr;
//     UIGroup->AddNewObject(imgTarget);
//     // Preload Lose Scene
//     deathBGMInstance = Engine::Resources::GetInstance().GetSampleInstance("astronomia.ogg");
//     Engine::Resources::GetInstance().GetBitmap("lose/benjamin-happy.png");
//     // Start BGM.
//     bgmId = AudioHelper::PlayBGM("village-explore.mp3");
// }
//
//
// void OceanScene::Terminate() {
//     // dipanggil pas scene selesai, kayak exit stage
//     // ini nge-stop bgm dan clean memory
//     AudioHelper::StopBGM(bgmId);
//     AudioHelper::StopSample(deathBGMInstance);
//     deathBGMInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
//     IScene::Terminate();
// }
//
// void OceanScene::Update(float deltaTime) {
//     // ini main loop per frame. all game logic jalan disini tiap tick
//     // spawn musuh, update status enemy, win/lose check, preview turret gerak, cheat code timer etc
//     // If we use deltaTime directly, then we might have Bullet-through-paper problem.
//     // Reference: Bullet-Through-Paper
//     if (SpeedMult == 0)
//         deathCountDown = -1;
//     else if (deathCountDown != -1)
//         SpeedMult = 1;
//     // Calculate danger zone.
//     std::vector<float> reachEndTimes;
//     for (auto &it : EnemyGroup->GetObjects()) {
//         reachEndTimes.push_back(dynamic_cast<Enemy *>(it)->reachEndTime);
//     }
//     // Can use Heap / Priority-Queue instead. But since we won't have too many enemies, sorting is fast enough.
//     std::sort(reachEndTimes.begin(), reachEndTimes.end());
//     float newDeathCountDown = -1;
//     int danger = lives;
//     for (auto &it : reachEndTimes) {
//         if (it <= DangerTime) {
//             danger--;
//             if (danger <= 0) {
//                 // Death Countdown
//                 float pos = DangerTime - it;
//                 if (it > deathCountDown) {
//                     // Restart Death Count Down BGM.
//                     AudioHelper::StopSample(deathBGMInstance);
//                     if (SpeedMult != 0)
//                         deathBGMInstance = AudioHelper::PlaySample("astronomia.ogg", false, AudioHelper::BGMVolume, pos);
//                 }
//                 float alpha = pos / DangerTime;
//                 alpha = std::max(0, std::min(255, static_cast<int>(alpha * alpha * 255)));
//                 dangerIndicator->Tint = al_map_rgba(255, 255, 255, alpha);
//                 newDeathCountDown = it;
//                 break;
//             }
//         }
//     }
//     deathCountDown = newDeathCountDown;
//     if (SpeedMult == 0)
//         AudioHelper::StopSample(deathBGMInstance);
//     if (deathCountDown == -1 && lives > 0) {
//         AudioHelper::StopSample(deathBGMInstance);
//         dangerIndicator->Tint.a = 0;
//     }
//     if (SpeedMult == 0)
//         deathCountDown = -1;
//     for (int i = 0; i < SpeedMult; i++) {
//         IScene::Update(deltaTime);
//
//
//         RinCharacter* rin = nullptr;
//         for (auto& obj : EffectGroup->GetObjects()) {
//             rin = dynamic_cast<RinCharacter*>(obj);
//             if (rin) break;
//         }        if (rin) {
//             for (auto& obj : EffectGroup->GetObjects()) {
//                 auto* house = dynamic_cast<House*>(obj);
//                 if (!house) continue;
//
//                 float dx = house->Position.x - rin->Position.x;
//                 float dy = house->Position.y - rin->Position.y;
//                 float distance = std::sqrt(dx * dx + dy * dy);
//                 if (distance < 64) { // 64 instead of BlockSize/2
//                     house->OnTouch();
//                 }
//             }
//         }
//         //RinCharacter* rin = nullptr;
//         for (auto& obj : EffectGroup->GetObjects()) {
//             rin = dynamic_cast<RinCharacter*>(obj);
//             if (rin) break;
//         }
//
//         if (rin) {
//             for (auto& obj : EffectGroup->GetObjects()) {
//                 auto* npc = dynamic_cast<NPC*>(obj);
//                 if (!npc) continue;
//
//                 float dx = npc->Position.x - rin->Position.x;
//                 float dy = npc->Position.y - rin->Position.y;
//                 float distance = std::sqrt(dx * dx + dy * dy);
//
//                 // Use a threshold for proximity, e.g., 64
//                 if (distance < 64) {
//                     npc->OnTouch();
//                 }
//             }
//         }
//
//
//
//
//         // Check if we should create new enemy.
//         ticks += deltaTime;
//         if (enemyWaveData.empty()) {
//             if (EnemyGroup->GetObjects().empty()) {
//                 // Free resources.
//                 /*delete TileMapGroup;
//                 delete GroundEffectGroup;
//                 delete DebugIndicatorGroup;
//                 delete TowerGroup;
//                 delete EnemyGroup;
//                 delete BulletGroup;
//                 delete EffectGroup;
//                 delete UIGroup;
//                 delete imgTarget;
//                 */
//                 // Win.
//
//
//                 //std::cout << "changing scene to win" << std::endl;
//                 //auto* win = dynamic_cast<WinScene*>(Engine::GameEngine::GetInstance().GetScene("win"));
//                 //if (win) win->SetFinalScore(money); // ini buat recording for scoreboard
//                 //Engine::GameEngine::GetInstance().ChangeScene("win");
//                 //return;
//             }
//             continue;
//         }
//         auto current = enemyWaveData.front();
//         if (ticks < current.second)
//             continue;
//         ticks -= current.second;
//         enemyWaveData.pop_front();
//         const Engine::Point SpawnCoordinate = Engine::Point(SpawnGridPoint.x * BlockSize + BlockSize / 2, SpawnGridPoint.y * BlockSize + BlockSize / 2);
//         Enemy *enemy;
//         switch (current.first) {
//             case 1:
//                 EnemyGroup->AddNewObject(enemy = new SoldierEnemy(SpawnCoordinate.x, SpawnCoordinate.y));
//                 break;
//             // TODO HACKATHON-3 (2/3): Add your new enemy here. (DONE)
//             case 2:
//                 EnemyGroup->AddNewObject(enemy = new PlaneEnemy(SpawnCoordinate.x, SpawnCoordinate.y));
//                 break;
//             case 3:
//                 EnemyGroup->AddNewObject(enemy = new TankEnemy(SpawnCoordinate.x, SpawnCoordinate.y));
//                 break;
//             case 4:
//                 EnemyGroup->AddNewObject(enemy = new AquaTankEnemy(SpawnCoordinate.x, SpawnCoordinate.y));
//                 break;
//             default:
//                 continue;
//         }
//         enemy->UpdatePath(mapDistance);
//         // Compensate the time lost.
//         enemy->Update(ticks);
//     }
//     if (preview) {
//         preview->Position = Engine::GameEngine::GetInstance().GetMousePosition();
//         // To keep responding when paused.
//         preview->Update(deltaTime);
//     }
//     // Automatically restore life label color if it shows damage red
//     if (UILives) {
//         ALLEGRO_COLOR alertRed = al_map_rgb(255, 40, 40);
//         ALLEGRO_COLOR current = UILives->Color;
//         if (current.r == alertRed.r && current.g == alertRed.g && current.b == alertRed.b) {
//             UILives->Color = al_map_rgb(255, 255, 255); // Reset to default white
//         }
//     }
//     if (cheatActive) {
//         cheatTimer += deltaTime;
//         if (cheatTimer >= 5.0f && cheatLabel) {
//             UIGroup->RemoveObject(cheatLabel->GetObjectIterator());
//             cheatLabel = nullptr;
//             cheatActive = false;
//         }
//     }
//     if (refundActive) {
//         refundTimer += deltaTime;
//         if (refundTimer >= 2.0f && refundLabel) {
//             UIGroup->RemoveObject(refundLabel->GetObjectIterator());
//             refundLabel = nullptr;
//             refundActive = false; // hide it
//         }
//     }
// }
//
// void OceanScene::Draw() const {
//
//     // buat gambar ulang seluruh scene (dipanggil tiap frame)
//     // kalo debug mode on, dia juga ngegambar jarak path musuh (BFS)
//     IScene::Draw();
//     if (DebugMode) {
//         // Draw reverse BFS distance on all reachable blocks.
//         for (int i = 0; i < MapHeight; i++) {
//             for (int j = 0; j < MapWidth; j++) {
//                 if (mapDistance[i][j] != -1) {
//                     // Not elegant nor efficient, but it's quite enough for debugging.
//                     Engine::Label label(std::to_string(mapDistance[i][j]), "pirulen.ttf", 32, (j + 0.5) * BlockSize, (i + 0.5) * BlockSize);
//                     label.Anchor = Engine::Point(0.5, 0.5);
//                     label.Draw();
//                     std::cout << "TileMapGroup drawing done.\n";
//                 }
//             }
//         }
//     }
// }
//
// void OceanScene::OnMouseDown(int button, int mx, int my) {
//     // pas mouse diklik (kiri/right), ini yg nanganin cancel turret preview
//     if ((button & 1) && !imgTarget->Visible && preview) {
//         // Cancel turret construct.
//         UIGroup->RemoveObject(preview->GetObjectIterator());
//         preview = nullptr;
//     }
//     IScene::OnMouseDown(button, mx, my);
// }
// void OceanScene ::OnMouseMove(int mx, int my) {
//     // update posisi target image biar kek sesuai posisi mouse
//     IScene::OnMouseMove(mx, my);
//     const int x = mx / BlockSize;
//     const int y = my / BlockSize;
//     if (!preview || x < 0 || x >= MapWidth || y < 0 || y >= MapHeight) {
//         imgTarget->Visible = false;
//         return;
//     }
//     imgTarget->Visible = true;
//     imgTarget->Position.x = x * BlockSize;
//     imgTarget->Position.y = y * BlockSize;
// }
//
// void OceanScene::OnMouseUp(int button, int mx, int my) {
//     // kalo mouse dilepas, handle logic buat naro turret atau shovel delete
//     // ini juga yang nentuin refund kalo pake shovel tool
//     IScene::OnMouseUp(button, mx, my);
//     if (!imgTarget->Visible) return;
//
//     const int x = mx / BlockSize;
//     const int y = my / BlockSize;
//
//     if (!(button & 1) || !preview) return;
//     ShovelTool* shovel = dynamic_cast<ShovelTool*>(preview);
//     if (shovel) {
//         Turret* target = nullptr;
//         for (auto& obj : TowerGroup->GetObjects()) {
//             Turret* turret = dynamic_cast<Turret*>(obj);
//             if (!turret || dynamic_cast<ShovelTool*>(turret)) continue;
//             int tx = floor(turret->Position.x / BlockSize);
//             int ty = floor(turret->Position.y / BlockSize);
//             if (tx == x && ty == y) {
//                 target = turret;
//                 break;
//             }
//         }
//         if (target) {
//             TowerGroup->RemoveObject(target->GetObjectIterator());
//             mapState[y][x] = TILE_GRASS;
//             int refund = 25;
//             EarnMoney(refund);
//             refundLabel = new Engine::Label("+$" + std::to_string(refund), "pirulen.ttf", 24, mx, my, 0, 255, 0);
//             refundLabel->Anchor = Engine::Point(0.5, 0.5);
//             UIGroup->AddNewObject(refundLabel);
//             refundTimer = 0.0f;
//             refundActive = true;
//             std::cout << "[DEBUG] Deleting turret at (" << x << "," << y << "), refund = $" << target->GetPrice() << std::endl;
//             keyStrokes.clear();
//             Engine::Sprite* sprite;
//             GroundEffectGroup->AddNewObject(sprite = new DirtyEffect("play/target-invalid.png", 1,
//                 x * BlockSize + BlockSize / 2,
//                 y * BlockSize + BlockSize / 2));
//             sprite->Rotation = 0;
//         }
//
//
//         UIGroup->RemoveObject(preview->GetObjectIterator());
//         preview = nullptr;
//         return;
//     }
//     // NORMAL TURRET PLACEMENT LOGIC
//     if (mapState[y][x] != TILE_OCCUPIED) {
//         if (!CheckSpaceValid(x, y)) {
//             Engine::Sprite* sprite;
//             GroundEffectGroup->AddNewObject(
//                 new DirtyEffect("play/target-invalid.png", 1,
//                     x * BlockSize + BlockSize / 2,
//                     y * BlockSize + BlockSize / 2)
//             );
//             return;
//         }
//
//         // Purchase and place turret
//         EarnMoney(-preview->GetPrice());
//         preview->GetObjectIterator()->first = false;
//         UIGroup->RemoveObject(preview->GetObjectIterator());
//         preview->Position.x = x * BlockSize + BlockSize / 2;
//         preview->Position.y = y * BlockSize + BlockSize / 2;
//         preview->Enabled = true;
//         preview->Preview = false;
//         preview->Tint = al_map_rgba(255, 255, 255, 255);
//         TowerGroup->AddNewObject(preview);
//         preview->Update(0);
//         preview = nullptr;
//
//         mapState[y][x] = TILE_OCCUPIED;
//         OnMouseMove(mx, my);
//     }
// }
//
//
// void OceanScene::OnKeyDown(int keyCode) {
//     //handle shortcut turret Q/W/E/R
// // juga logic cheat code masuk disini (bisa spawn plane + 10k)
//     std::cout << "Pressed: " << keyCode << std::endl;
//     IScene::OnKeyDown(keyCode);
//     if (keyCode == ALLEGRO_KEY_W || keyCode == 84 || keyCode == ALLEGRO_KEY_UP) keyUpDown = true;
//     if (keyCode == ALLEGRO_KEY_S || keyCode == 85 || keyCode == ALLEGRO_KEY_DOWN) keyDownDown = true;
//     if (keyCode == ALLEGRO_KEY_A || keyCode == 82 || keyCode == ALLEGRO_KEY_LEFT) keyLeftDown = true;
//     if (keyCode == ALLEGRO_KEY_D || keyCode == 83 || keyCode == ALLEGRO_KEY_RIGHT) keyRightDown = true;
//     if (keyCode == ALLEGRO_KEY_TAB) {
//         DebugMode = !DebugMode;
//     }
//     else {
//         keyStrokes.push_back(keyCode);
//         if (keyStrokes.size() > code.size())
//             keyStrokes.pop_front();
//
//         bool matched = keyStrokes.size() >= code.size() &&
//             std::equal(code.begin(), code.end(), std::prev(keyStrokes.end(), code.size()));
//
//         if (matched) {
//             AddNewObject(new Plane());
//             EarnMoney(10000);
//             cheatLabel = new Engine::Label("U DISCOVERED THE CHEAT CODE!", "pirulen.ttf", 48,
//                                                 640, 300, 255, 0, 0, 255);
//             cheatLabel->Anchor = Engine::Point(0.5, 0.5);
//             UIGroup->AddNewObject(cheatLabel);
//             cheatTimer = 0.0f;         // timer for the text
//             cheatActive = true;        // to track it lah peppi
//             std::cout << "Cheat Code Matched, adding 10k to money and spawning plane\n";
//             keyStrokes.clear();
//         }
//     }
//     if (keyCode == ALLEGRO_KEY_Q) {
//         // Hotkey for MachineGunTurret.
//         UIBtnClicked(0);
//     } else if (keyCode == ALLEGRO_KEY_W) {
//         // Hotkey for LaserTurret.
//         UIBtnClicked(1);
//     } else if (keyCode == ALLEGRO_KEY_E) {
//         // Hotkey for FireTurret.
//         UIBtnClicked(2);
//     } else if (keyCode == ALLEGRO_KEY_R) {
//         // Hotkey for RocketTurretw.
//         UIBtnClicked(3);
//     }
//     else if (keyCode >= ALLEGRO_KEY_0 && keyCode <= ALLEGRO_KEY_9) {
//         // Hotkey for Speed up.
//         SpeedMult = keyCode - ALLEGRO_KEY_0;
//     }
// }
//
// void OceanScene::OnKeyUp(int keyCode) {
//     IScene::OnKeyUp(keyCode);
//
//     if (keyCode == ALLEGRO_KEY_W || keyCode == 84 || keyCode == ALLEGRO_KEY_UP) keyUpDown = false;
//     if (keyCode == ALLEGRO_KEY_S || keyCode == 85 || keyCode == ALLEGRO_KEY_DOWN) keyDownDown = false;
//     if (keyCode == ALLEGRO_KEY_A || keyCode == 82 || keyCode == ALLEGRO_KEY_LEFT) keyLeftDown = false;
//     if (keyCode == ALLEGRO_KEY_D || keyCode == 83 || keyCode == ALLEGRO_KEY_RIGHT) keyRightDown = false;
// }
//
// void OceanScene::Hit() {
//     // ini dipanggil kalo enemy tembus base. ngurangin nyawa dan update UI life
//     lives--;
//     if (UILives) {
//         UILives->Text = std::string("Life ") + std::to_string(lives);
//         UILives->Color = al_map_rgb(255, 50, 50);
//     }
//     if (lives <= 0) {
//         Engine::GameEngine::GetInstance().ChangeScene("lose");
//     }
// }
//
//
//
// int OceanScene::GetMoney() const {
//     // getter buat current money. i can call dari luar
//     return money;
// }
//
// void OceanScene::EarnMoney(int money) {
//     // update label UI money bisa nambah bisa ngurang
//     this->money += money;
//     UIMoney->Text = std::string("$") + std::to_string(this->money);
// }
//
// void OceanScene::ReadMap() {
//     // load file map.txt jd mapState
//     // ngegambar tile floor / dirt ke TileMapGroup
//     std::string filename = std::string("Resource/map") + std::to_string(MapId) + ".txt";
//     // Read map file.
//     char c;
//     std::vector<int> mapData;
//     std::ifstream fin(filename);
//     while (fin >> c) {
//         switch (c) {
//             case '0': mapData.push_back(0); break;
//             case '1': mapData.push_back(1); break;
//             case '2': mapData.push_back(2); break;
//             case '3': mapData.push_back(3); break;
//             case '4': mapData.push_back(4); break;
//             case '5': mapData.push_back(5); break;
//             case '6': mapData.push_back(6); break;
//             case '7': mapData.push_back(7); break;
//             case '\n':
//             case '\r':
//                 if (static_cast<int>(mapData.size()) / MapWidth != 0)
//                     throw std::ios_base::failure("Map data is corrupted.");
//                 break;
//             default: throw std::ios_base::failure("Map data is corrupted.");
//         }
//     }
//     fin.close();
//     // Validate map data.
//     if (static_cast<int>(mapData.size()) != MapWidth * MapHeight)
//         throw std::ios_base::failure("Map data is corrupted.");
//     // Store map in 2d array.
//     mapState = std::vector<std::vector<TileType>>(MapHeight, std::vector<TileType>(MapWidth));
//     for (int i = 0; i < MapHeight; i++) {
//         for (int j = 0; j < MapWidth; j++) {
//             const int num = mapData[i * MapWidth + j];
//             //mapState[i][j] = num ? TILE_FLOOR : TILE_DIRT;
//             // if (1) {
//             //     TileMapGroup->AddNewObject(new Engine::Image("play/floor.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize)); // previously floor.png
//             // }
//             // if (2) {
//             //     TileMapGroup->AddNewObject(new Engine::Image("play/grass.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
//             // }
//             switch(num) {
//                 case 0: mapState[i][j] = TILE_GRASS;
//                     TileMapGroup->AddNewObject( new Engine::Image("play/grass.png", j*BlockSize, i*BlockSize, BlockSize, BlockSize) );
//                     break;
//                 case 1: mapState[i][j] = TILE_FLOOR;
//                     TileMapGroup->AddNewObject(new Engine::Image("play/floor.png", j*BlockSize, i*BlockSize, BlockSize, BlockSize));
//                     break;
//                 case 2: mapState[i][j] = TILE_FENCE;
//                     TileMapGroup->AddNewObject(new Engine::Image("play/fence.png", j*BlockSize, i*BlockSize, BlockSize, BlockSize));
//                     break;
//                 case 3: mapState[i][j] = TILE_FENCE_RIGHT;
//                     TileMapGroup->AddNewObject(new Engine::Image("play/fence_right.png", j*BlockSize, i*BlockSize, BlockSize, BlockSize));
//                     break;
//                 case 4: mapState[i][j] = TILE_FENCE_DOWN;
//                     TileMapGroup->AddNewObject(new Engine::Image("play/fence_down.png", j*BlockSize, i*BlockSize, BlockSize, BlockSize));
//                     break;
//                 case 5: mapState[i][j] = TILE_FENCE_LEFT;
//                     TileMapGroup->AddNewObject(new Engine::Image("play/fence_left.png", j*BlockSize, i*BlockSize, BlockSize, BlockSize));
//                     break;
//                 case 6: mapState[i][j] = TILE_BOOK;
//                     TileMapGroup->AddNewObject(new Engine::Image("play/house_book.png", j*BlockSize, i*BlockSize, BlockSize, BlockSize));
//                     break;
//                 case 7: mapState[i][j] = TILE_INVENTORY;
//                     TileMapGroup->AddNewObject(new Engine::Image("play/house_inventory.png", j*BlockSize, i*BlockSize, BlockSize, BlockSize));
//                     break;
//                     //case 2: mapState[i][j] = TILE_GRASS;
//                     //TileMapGroup->AddNewObject(new Engine::Image("play/grass.png", j*BlockSize, i*BlockSize, BlockSize, BlockSize) );
//                     //break;
//                 //case 3: mapState[i][j] = TILE_TREE;
//                    // TileMapGroup->AddNewObject(new Engine::Image("play/tree.png", j*BlockSize, i*BlockSize, BlockSize, BlockSize) );
//                    // break;
//                 //default:
//                     //break;
//             }
//         }
//     }
// }
//
// void OceanScene::ReadEnemyWave() {
//     // load file enemyX.txt buat wave musuh
//     // tiap baris disimpan sebagai pair (tipe musuh, interval muncul)
//     std::string filename = std::string("Resource/enemy") + std::to_string(MapId) + ".txt";
//     // Read enemy file.
//     float type, wait, repeat;
//     enemyWaveData.clear();
//     std::ifstream fin(filename);
//     while (fin >> type && fin >> wait && fin >> repeat) {
//         for (int i = 0; i < repeat; i++)
//             enemyWaveData.emplace_back(type, wait);
//     }
//     fin.close();
// }
//
// void OceanScene::ConstructUI() {
//     // setup tombol-tombol turret, text UI (stage, money, lives)
//     // Background
//     UIGroup->AddNewObject(new Engine::Image("play/wooden_forest.png", 1472, 0, 320, 1216));
//     // Text
//     UIGroup->AddNewObject(new Engine::Label(std::string("Stage ") + std::to_string(MapId), "pirulen.ttf", 32, 1486, 0));
//     UIGroup->AddNewObject(UIMoney = new Engine::Label(std::string("$") + std::to_string(money), "pirulen.ttf", 24, 1486, 48));
//     UIGroup->AddNewObject(UILives = new Engine::Label(std::string("Life ") + std::to_string(lives), "pirulen.ttf", 24, 1486, 88));
//     TurretButton *btn;
//     // Button 1
//     btn = new TurretButton("play/floor.png", "play/dirt.png",
//                            Engine::Sprite("play/tower-base.png", 1486, 136, 0, 0, 0, 0),
//                            Engine::Sprite("play/turret-1.png", 1486, 136 - 8, 0, 0, 0, 0), 1486, 136, MachineGunTurret::Price);
//     // Reference: Class Member Function Pointer and std::bind.
//     btn->SetOnClickCallback(std::bind(&OceanScene::UIBtnClicked, this, 0));
//     UIGroup->AddNewControlObject(btn);
//
//     // Button 2
//     btn = new TurretButton("play/floor.png", "play/dirt.png",
//                            Engine::Sprite("play/tower-base.png", 1562, 136, 0, 0, 0, 0),
//                            Engine::Sprite("play/turret-2.png", 1562, 136 - 8, 0, 0, 0, 0), 1562, 136, LaserTurret::Price);
//     btn->SetOnClickCallback(std::bind(&OceanScene::UIBtnClicked, this, 1));
//     UIGroup->AddNewControlObject(btn);
//
//     // fire turret
//     btn = new TurretButton("play/floor.png", "play/dirt.png",
//                        Engine::Sprite("play/tower-base.png", 1636, 136, 0, 0, 0, 0),
//                        Engine::Sprite("play/turret-fire.png", 1636, 128, 0, 0, 0, 0), 1636, 136, FireTurret::Price);
//     btn->SetOnClickCallback(std::bind(&OceanScene::UIBtnClicked, this, 2)); // ID = 2
//     UIGroup->AddNewControlObject(btn);
//
//     //rocket turret
//     btn = new TurretButton("play/floor.png", "play/dirt.png",
//                        Engine::Sprite("play/tower-base.png", 1714, 136, 0, 0, 0, 0),
//                        Engine::Sprite("play/turret-6.png", 1714, 136 - 8, 0, 0, 0, 0), 1714, 136, RocketTurret::Price);
//     btn->SetOnClickCallback(std::bind(&OceanScene::UIBtnClicked, this, 3));
//     UIGroup->AddNewControlObject(btn);
//
//     //shovel btn
//     btn = new TurretButton("play/floor.png", "play/dirt.png",
//                        Engine::Sprite("play/shovel-base.png", 1486, 212, 0, 0, 0, 0),
//                        Engine::Sprite("play/shovel.png", 1486, 212 - 8, 0, 0, 0, 0), 1486, 212, ShovelTool::Price);
//     btn->SetOnClickCallback(std::bind(&OceanScene::UIBtnClicked, this, 4));
//     UIGroup->AddNewControlObject(btn);
//
//     int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
//     int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
//     int shift = 135 + 25;
//     dangerIndicator = new Engine::Sprite("play/benjamin.png", w - shift, h - shift);
//     dangerIndicator->Tint.a = 0;
//     UIGroup->AddNewObject(dangerIndicator);
// }
//
// void OceanScene::UIBtnClicked(int id) {
//     // pas tombol turret diklik, ini yg buat turret preview muncul
//     if (preview)
//         UIGroup->RemoveObject(preview->GetObjectIterator());
//     if (id == 0 && money >= MachineGunTurret::Price)
//         preview = new MachineGunTurret(0, 0);
//     else if (id == 1 && money >= LaserTurret::Price)
//         preview = new LaserTurret(0, 0);
//     //fireTurret new yeah
//     else if (id == 2 && money >= FireTurret::Price)
//         preview = new FireTurret(0, 0);
//     else if (id == 3 && money >= RocketTurret::Price)
//         preview = new RocketTurret(0, 0);
//     else if (id == 4 && money >= ShovelTool::Price)
//         preview = new ShovelTool(0, 0);
//     if (!preview)
//         return;
//     preview->Position = Engine::GameEngine::GetInstance().GetMousePosition();
//     preview->Tint = al_map_rgba(255, 255, 255, 200);
//     preview->Enabled = false;
//     preview->Preview = true;
//     UIGroup->AddNewObject(preview);
//     OnMouseMove(Engine::GameEngine::GetInstance().GetMousePosition().x, Engine::GameEngine::GetInstance().GetMousePosition().y);
// }
//
// bool OceanScene::CheckSpaceValid(int x, int y) {
//     // ngecek bisa naro turret di (x, y) gak plus gak boleh blokir jalan musuh (appliednya mulai level 2)
//     // pake BFS cek pathing enemy
//     if (x < 0 || x >= MapWidth || y < 0 || y >= MapHeight)
//         return false;
//     auto map00 = mapState[y][x];
//     mapState[y][x] = TILE_OCCUPIED;
//     std::vector<std::vector<int>> map = CalculateBFSDistance();
//     mapState[y][x] = map00;
//     if (map[0][0] == -1)
//         return false;
//     for (auto &it : EnemyGroup->GetObjects()) {
//         Engine::Point pnt;
//         pnt.x = floor(it->Position.x / BlockSize);
//         pnt.y = floor(it->Position.y / BlockSize);
//         if (pnt.x < 0) pnt.x = 0;
//         if (pnt.x >= MapWidth) pnt.x = MapWidth - 1;
//         if (pnt.y < 0) pnt.y = 0;
//         if (pnt.y >= MapHeight) pnt.y = MapHeight - 1;
//         if (map[pnt.y][pnt.x] == -1)
//             return false;
//     }
//     // All enemy have path to exit.
//     mapState[y][x] = TILE_OCCUPIED;
//     mapDistance = map;
//     for (auto &it : EnemyGroup->GetObjects())
//         dynamic_cast<Enemy *>(it)->UpdatePath(mapDistance);
//     return true;
// }
//
// std::vector<std::vector<int>> OceanScene::CalculateBFSDistance() {
//     // ini BFS buat nyari jarak dari tile ke goal (ujung kanan bawah)
//     // dipake musuh buat update path tiap kali ada turret ditaro
//     // Reverse BFS to find path.
//     std::vector<std::vector<int>> map(MapHeight, std::vector<int>(std::vector<int>(MapWidth, -1)));
//     std::queue<Engine::Point> que;
//     // Push end point.
//     // BFS from end point.
//     if (mapState[MapHeight - 1][MapWidth - 1] != TILE_GRASS)
//         return map;
//     que.push(Engine::Point(MapWidth - 1, MapHeight - 1));
//     map[MapHeight - 1][MapWidth - 1] = 0;
//     while (!que.empty()) {
//         Engine::Point p = que.front();
//         que.pop();
//         // TODO PROJECT-1 (1/1): Implement a BFS starting from the most right-bottom block in the map.
//         //               For each step you should assign the corresponding distance to the most right-bottom block.
//         //               mapState[y][x] is TILE_DIRT if it is empty.
//
//         for (const auto& offset : directions) { //bfs for enemy(trying all possible neighbor first then move to next step)
//             //direction is olr listed as up, down, right and left
//             int next_x = p.x + offset.x; //calculating neighbor in x direction
//             int next_y = p.y + offset.y; // the same but for y direction
//             // out of bounds check, kalo diluar limit ya skip
//             if (next_x < 0 || next_x >= MapWidth || next_y < 0 || next_y >= MapHeight)
//                 continue;
//             // availability check, kalo gakosong skip, kalo walls skip
//             if (mapState[next_y][next_x] != TILE_GRASS || map[next_y][next_x] != -1)
//                 continue;
//
//             // distance formula= first until last diitung
//             //bfs movement(+1 to jalan)
//             map[next_y][next_x] = map[p.y][p.x] + 1;
//             //bis itu push bfs further from available tile after checking them
//             que.push(Engine::Point(next_x, next_y));
//         }
//     }
//     return map;
// }
