#include "VillageScene.hpp"

#include <fstream>
#include <stdexcept>
#include <string>
#include <iostream>

#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "UI/Component/ImageButton.hpp"
#include "Engine/Resources.hpp"
#include "Engine/AudioHelper.hpp"


void VillageScene::Initialize() {
    std::cerr << "VillageScene initializing..." << std::endl;

    mapState.clear();

    // Create rendering group for map
    AddNewObject(TileMapGroup = new Engine::Group());
    AddNewControlObject(UIGroup = new Engine::Group());

    // Load map file and draw it
    ReadMap();

    // (Optional) Play background music
    std::cerr << "Before playing BGM, bgmInstance is " << bgmInstance.get() << std::endl;
    bgmInstance = AudioHelper::PlaySample("village-explore.mp3", true, AudioHelper::BGMVolume);
    std::cerr << "After playing BGM, bgmInstance is " << bgmInstance.get() << std::endl;
}

void VillageScene::Terminate() {
    if (bgmInstance) {
        std::cerr << "Stopping BGM instance." << std::endl;
        AudioHelper::StopSample(bgmInstance);
        bgmInstance.reset();
    } else {
        std::cerr << "No BGM instance to stop." << std::endl;
    }

}

void VillageScene::ReadMap() {
    std::string filename = "Resource/village_map.txt"; // Updated to match your file
    std::ifstream fin(filename);
    if (!fin) throw std::ios_base::failure("Cannot open map file: " + filename);

    char c;
    std::vector<bool> mapData;

    while (fin.get(c)) {
        if (c == '0') mapData.push_back(false);
        else if (c == '1') mapData.push_back(true);
        else if (c == '\n' || c == '\r') continue;  // skip newlines and carriage returns
        else if (c == ' ' || c == '\t') continue;   // optionally skip spaces and tabs
        else throw std::ios_base::failure("Invalid character in map.");
    }

    fin.close();

    std::cerr << "Map size read: " << mapData.size() << std::endl;
    std::cerr << "Expected size: " << MapWidth * MapHeight << std::endl;

    if ((int)mapData.size() != MapWidth * MapHeight)
        throw std::ios_base::failure("Map size mismatch. Expected: " +
            std::to_string(MapWidth * MapHeight) + ", Got: " + std::to_string(mapData.size()));

    // Fill mapState and draw tiles
    mapState = std::vector<std::vector<TileType>>(MapHeight, std::vector<TileType>(MapWidth));

    for (int i = 0; i < MapHeight; ++i) {
        for (int j = 0; j < MapWidth; ++j) {
            int index = i * MapWidth + j;
            bool isFloor = mapData[index];
            mapState[i][j] = isFloor ? TILE_FLOOR : TILE_DIRT;

            std::string imagePath = isFloor ? "play/tree(32x32).png" : "play/grass.png";
            TileMapGroup->AddNewObject(new Engine::Image(imagePath, j * BlockSize, i * BlockSize, BlockSize, BlockSize));
        }
    }
}