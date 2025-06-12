#ifndef VILLAGESCENE_HPP
#define VILLAGESCENE_HPP

#include "PlayScene.hpp"
class VillageScene : public PlayScene {

public:

    void Initialize() override;
    bool IsTileWalkable(int tileType) const override ;
    virtual void ReadMap() override;
};

#endif