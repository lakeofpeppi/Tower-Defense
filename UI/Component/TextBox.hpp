//
// Created by Angelie Melisa on 2025/5/24.
//

#ifndef TEXTBOX_HPP
#define TEXTBOX_HPP
#pragma once
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_color.h>
#include <string>
#include "Engine/IControl.hpp"
#include "Engine/IObject.hpp"
#include "Engine/Point.hpp"

namespace Engine {
    class TextBox final : public IObject, public IControl {
    public:
        std::string Text;
        std::string FontName;
        int FontSize;
        ALLEGRO_COLOR FontColor;
        Engine::Point Anchor;

        TextBox(std::string text, std::string font, int fontSize, float x, float y, unsigned char r, unsigned char g, unsigned char b, unsigned char a, float anchorX = 0, float anchorY = 0);
        void Draw() const override;
        void OnKeyDown(int keyCode) override;
    };
}

#endif //TEXTBOX_HPP
