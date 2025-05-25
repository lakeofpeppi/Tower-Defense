//
// Created by Angelie Melisa on 2025/5/26.
//
// ShovelTool.cpp
#include "ShovelTool.hpp"
#include "Engine/Group.hpp"
#include "Engine/Resources.hpp"

#include "Engine/GameEngine.hpp"
#include "Engine/Point.hpp"
#include "Engine/Collider.hpp"
#include "Scene/PlayScene.hpp"
#include "Turret/Turret.hpp"
#include "ShovelTool.hpp"
#include <allegro5/allegro_primitives.h>
#include "UI/Component/ShovelTool.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Sprite.hpp"
#include "Scene/PlayScene.hpp"
#include "Turret/Turret.hpp"
#include <iostream>

ShovelTool::ShovelTool(float x, float y, PlayScene* scene) :
    ImageButton("play/shovel.png", "play/shovel.png", x, y, 64, 64), // use same image for in and out
    scene(scene), dragging(false) {
}
void ShovelTool::OnClick(int button, int mx, int my) {
    if (button == 1) {
        std::cout << "Shovel clicked at " << mx << "," << my << std::endl;
        scene->Mode = PlayScene::PlayMode::SHOVEL;

        // Optional: preview shovel icon follow mouse
        if (scene->shovelPreview) {
            scene->UIGroup->RemoveObject(scene->shovelPreview->GetObjectIterator());
        }

        scene->shovelPreview = new Engine::Sprite("play/shovel.png", mx, my);
        scene->shovelPreview->Anchor = Engine::Point(0.5, 0.5);
        scene->UIGroup->AddNewObject(scene->shovelPreview);
    }
}
void ShovelTool::OnMouseDown(int button, int mx, int my) {
    if ((button & 1)) {
        dragging = true;
        std::cout << "[DEBUG] Shovel clicked. Dragging started at (" << mx << ", " << my << ")\n";

        if (scene->shovelPreview) {
            scene->UIGroup->RemoveObject(scene->shovelPreview->GetObjectIterator());
        }

        scene->shovelPreview = new Engine::Sprite("play/shovel.png", mx, my);
        scene->shovelPreview->Anchor = Engine::Point(0.5, 0.5);
        scene->UIGroup->AddNewObject(scene->shovelPreview);
    }
}

void ShovelTool::StartDrag() {
    dragging = true;
}

void ShovelTool::StopDrag() {
    dragging = false;
}

bool ShovelTool::IsDragging() const {
    return dragging;
}
