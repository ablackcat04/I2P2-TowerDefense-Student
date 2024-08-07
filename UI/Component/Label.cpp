#include <allegro5/allegro_font.h>
#include <memory>

#include "Engine/IObject.hpp"
#include "Engine/Resources.hpp"
#include "Label.hpp"

int flash = 0;

namespace Engine {
	Label::Label(const std::string& text, const std::string& font, int fontSize, float x, float y, unsigned char r, unsigned char g, unsigned char b, unsigned char a, float anchorX, float anchorY) :
		IObject(x, y, 0, 0, anchorX, anchorY), font(Resources::GetInstance().GetFont(font, fontSize)), Text(text), Color(al_map_rgba(r, g, b, a)) {
        t = nullptr;
	}
    Label::Label(std::string* ta, const std::string& font, int fontSize, float x, float y, unsigned char r,
                 unsigned char g, unsigned char b, unsigned char a, float anchorX, float anchorY, bool is_input):
            IObject(x, y, 0, 0, anchorX, anchorY), font(Resources::GetInstance().GetFont(font, fontSize)), Text(""), t(ta), Color(al_map_rgba(r, g, b, a)), is_input(is_input) {
    }
	void Label::Draw() const {
        ++flash;

        if (t == nullptr) {
            al_draw_text(font.get(), Color, Position.x - Anchor.x * GetTextWidth(),
                         Position.y - Anchor.y * GetTextHeight(), 0, Text.c_str());
        } else if (flash % 96 >= 48 || is_input == false) {
            al_draw_text(font.get(), Color, Position.x - Anchor.x * GetTextWidth(),
                         Position.y - Anchor.y * GetTextHeight(), 0, t->c_str());
        } else {
            al_draw_text(font.get(), Color, Position.x - Anchor.x * GetTextWidth(),
                         Position.y - Anchor.y * GetTextHeight(), 0, (*t + "|").c_str());
        }
    }
	int Label::GetTextWidth() const {
		return al_get_text_width(font.get(), Text.c_str());
	}
	int Label::GetTextHeight() const {
		return al_get_font_line_height(font.get());
	}
}
