//
// Created by Angelie Melisa on 2025/6/12.
//

#ifndef FORESTSCENE_HPP
#define FORESTSCENE_HPP

#include "PlayScene.hpp"
class ForestScene : public PlayScene {
    bool _didTransition = false;
public:

    void Initialize() override;
    bool IsTileWalkable(int tileType) const override ;
    void ReadMap() override;
    TileType GetDefaultWalkableTile() const override;
    //void Transition() override;
    void Transition() override;
    void Update(float deltaTime) override;
    std::string GetMapImagePath() const override;
    void CloseMap() override;

};


#endif //FORESTSCENE_HPP
