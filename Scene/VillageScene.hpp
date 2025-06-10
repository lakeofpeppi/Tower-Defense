#ifndef VILLAGE_SCENE_HPP
#define VILLAGE_SCENE_HPP

#include <vector>
#include <memory>

#include "Engine/Group.hpp"
#include "Engine/AudioHelper.hpp"
#include "Engine/IScene.hpp"  // Assuming VillageScene inherits from IScene
#include "Engine/Resources.hpp" // For the tile images

// TileType enum, inferred from your code
enum TileType {
    TILE_DIRT,
    TILE_FLOOR
};

class VillageScene : public Engine::IScene {
public:
    VillageScene() = default;
    virtual ~VillageScene() = default;

    void Initialize() override;
    void Terminate() override;

private:
    void ReadMap();

    std::vector<std::vector<TileType>> mapState;

    // Groups for rendering and UI
    Engine::Group* TileMapGroup = nullptr;
    Engine::Group* UIGroup = nullptr;

    // Background music instance handle
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;

    // Map dimensions and tile size (should be defined somewhere globally or in this class)
    static constexpr int MapWidth = 28;  // Adjust to your actual map width
    static constexpr int MapHeight = 19; // Adjust to your actual map height
    static constexpr int BlockSize = 64; // Tile size in pixels
};

#endif // VILLAGE_SCENE_HPP
