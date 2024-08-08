#include <memory>
#include "Engine/GameEngine.hpp"
#include "Engine/Resources.hpp"
#include "ToggledTextButton.hpp"

namespace Engine {

    ToggledTextButton::ToggledTextButton(const std::string &text_input, bool *control, const float &x, const float &y,
                                         ALLEGRO_COLOR default_color, ALLEGRO_COLOR hovered_color, ALLEGRO_COLOR pressed_color)
        : text(text_input), ctrl(control), x(x), y(y), default_color(default_color), hovered_color(hovered_color), pressed_color(pressed_color)
    {
        font = al_load_font("Resource/fonts/BoutiqueBitmap7x7_1.7.ttf", 32, 0);
        w = al_get_text_width(font, text_input.c_str());
        h = 32;
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
                al_draw_text(font, default_color, x, y, 0, text.c_str());
            } else {
                al_draw_text(font, pressed_color, x, y, 0, text.c_str());
            }
        } else {
            al_draw_text(font, hovered_color, x, y, 0, text.c_str());
        }
    }
}
