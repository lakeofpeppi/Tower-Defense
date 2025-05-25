//
// Created by Angelie Melisa on 2025/5/26.
//

#ifndef SHOVELTOOL_HPP
#define SHOVELTOOL_HPP
#pragma once
#include "Engine/IObject.hpp"
#include "Engine/Sprite.hpp"
#include "Engine/Point.hpp"
#include "Scene/PlayScene.hpp"
#include "UI/Component/ImageButton.hpp"

class ShovelTool : public Engine::ImageButton {
private:
    PlayScene* scene;
    bool dragging = false;

public:
    ShovelTool(float x, float y, PlayScene* scene);
    void OnClick(int button, int mx, int my);
    void StartDrag();
    void StopDrag();
    bool IsDragging() const;
    void OnMouseDown(int button, int mx, int my) override;


};
#endif //SHOVELTOOL_HPP
