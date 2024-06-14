//
// Created by wuru8 on 2024/6/14.
//

#include "ToggledTextButton.hpp"

#include <functional>
#include <memory>

#include "Engine/Collider.hpp"
#include "Engine/GameEngine.hpp"
#include "Image.hpp"
#include "Engine/Point.hpp"
#include "Engine/Resources.hpp"

namespace Engine {

    ToggledTextButton::ToggledTextButton(const std::string &text_input, bool *control, const float &x, const float &y,
                                         ALLEGRO_COLOR default_color, ALLEGRO_COLOR hovered_color, ALLEGRO_COLOR pressed_color)
        :text(text_input), ctrl(control), x(x), y(y), dft_color(default_color), hvd_color(hovered_color), psd_color(pressed_color) {
        font = al_load_font("Resource/fonts/BoutiqueBitmap7x7_1.7.ttf", 20, 0);
        w = al_get_text_width(font, text_input.c_str());
        h = 20;
    }

    ToggledTextButton::~ToggledTextButton() {
        al_destroy_font(font);
    }

    void ToggledTextButton::OnMouseDown(int button, int mx, int my) {
        if (mouseIn) {
            *ctrl = !(*ctrl);
        }
    }

    void ToggledTextButton::OnMouseMove(int mx, int my) {
        if (x <= mx && mx <= x+w && y <= my && my <= y+h) {
            mouseIn = true;
        } else {
            mouseIn = false;
        }
    }

    void ToggledTextButton::Draw() const {
        if (!mouseIn) {
            if (*ctrl == false) {
                al_draw_text(font, dft_color, x, y, 0, text.c_str());
            } else {
                al_draw_text(font, psd_color, x, y, 0, text.c_str());
            }
        } else {
            al_draw_text(font, hvd_color, x, y, 0, text.c_str());
        }
    }
}
