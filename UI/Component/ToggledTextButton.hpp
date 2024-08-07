#ifndef INC_2024_I2P2_TOWERDEFENSE_TOGGLEDTEXTBUTTON_HPP
#define INC_2024_I2P2_TOWERDEFENSE_TOGGLEDTEXTBUTTON_HPP

#include <allegro5/bitmap.h>
#include <functional>
#include <memory>
#include <string>

#include "Engine/IControl.hpp"
#include "Image.hpp"
#include "Label.hpp"

namespace Engine {
    class ToggledTextButton: public IObject, public IControl {
    protected:
        // Determines whether mouse is in the button.
        bool mouseIn = false;
        bool* ctrl = nullptr;

        std::string text;

        int w;
        int h;

        ALLEGRO_COLOR dft_color;
        ALLEGRO_COLOR hvd_color;
        ALLEGRO_COLOR psd_color;

        ALLEGRO_FONT* font;

        float x;
        float y;

    public:
        // Whether the button can be pressed.
        bool Enabled = true;
        explicit ToggledTextButton(const std::string& text_input, bool *control, const float& x, const float& y, ALLEGRO_COLOR default_color, ALLEGRO_COLOR hovered_color, ALLEGRO_COLOR pressed_color);
        ~ToggledTextButton();
        void OnMouseDown(int button, int mx, int my) override;
        void OnMouseMove(int mx, int my) override;
        void Draw() const override;
    };
}

#endif //INC_2024_I2P2_TOWERDEFENSE_TOGGLEDTEXTBUTTON_HPP
