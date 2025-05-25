#include "TextBox.hpp"
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include "Engine/GameEngine.hpp"
#include "Engine/Resources.hpp"
#include "Engine/IObject.hpp"
#include "Engine/Point.hpp"
#include <iostream>

namespace Engine {
    TextBox::TextBox(std::string text, std::string font, int fontSize, float x, float y,
                     unsigned char r, unsigned char g, unsigned char b, unsigned char a,
                     float anchorX, float anchorY) : IObject(x, y, 0, 0, anchorX, anchorY), Text(text), FontName(font), FontSize(fontSize) {
        FontColor = al_map_rgba(r, g, b, a);
    }


    void TextBox::Draw() const {
        auto font = Resources::GetInstance().GetFont(FontName, FontSize);
        float width = 300;
        float height = al_get_font_line_height(font.get()) + 20;

        //box sizing
        const_cast<TextBox*>(this)->Size.x = width;
        const_cast<TextBox*>(this)->Size.y = height;

        // background box
        al_draw_filled_rectangle(
            Position.x - width / 2, Position.y - height / 2,
            Position.x + width / 2, Position.y + height / 2,
            al_map_rgba(0, 0, 0, 180) // semi-transparent black bg
        );

        // border
        al_draw_rectangle(
            Position.x - width / 2, Position.y - height / 2,
            Position.x + width / 2, Position.y + height / 2,
            al_map_rgb(255, 255, 255), 2.0 // white border
        );

        // text input
        al_draw_text(font.get(), FontColor,
            Position.x, Position.y - al_get_font_line_height(font.get()) / 2,
            ALLEGRO_ALIGN_CENTER, Text.c_str());
    }


    void TextBox::OnKeyDown(int keyCode) {
        if (keyCode == ALLEGRO_KEY_BACKSPACE) {
            if (!Text.empty())
                Text.pop_back();
        }
        else if (keyCode >= ALLEGRO_KEY_A && keyCode <= ALLEGRO_KEY_Z) {
            Text += static_cast<char>('A' + (keyCode - ALLEGRO_KEY_A)); // Capital letters
        }
        else if (keyCode >= ALLEGRO_KEY_0 && keyCode <= ALLEGRO_KEY_9) {
            Text += static_cast<char>('0' + (keyCode - ALLEGRO_KEY_0)); // Digits
        }
        else if (keyCode == ALLEGRO_KEY_SPACE) {
            Text += ' ';
        }
    }

}
