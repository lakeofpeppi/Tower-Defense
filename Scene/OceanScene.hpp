#ifndef OCEANSCENE_HPP
#define OCEANSCENE_HPP

#include "PlayScene.hpp"
class OceanScene : public PlayScene {
    bool _didTransition = false;
public:

    void Initialize() override;
    bool IsTileWalkable(int tileType) const override ;
    void ReadMap() override;
    TileType GetDefaultWalkableTile() const override;
    void Transition() override;
    void Update(float deltaTime) override;
    std::string GetMapImagePath() const override;
    void CloseMap() override;

};
#endif