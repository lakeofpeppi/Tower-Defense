//
// Created by Angelie Melisa on 2025/5/26.
//

#ifndef SHOVELTOOL_HPP
#define SHOVELTOOL_HPP

#include "Turret.hpp"

class ShovelTool : public Turret {
public:
    static const int Price;
    ShovelTool(float x, float y);
    void CreateBullet() override;
};

#endif //SHOVELTOOL_HPP
