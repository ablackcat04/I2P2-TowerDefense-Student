#include "FPSLabel.hpp"

namespace Engine {
    FPSLabel::FPSLabel(const std::string &font, int fontSize, float x, float y, unsigned char r,
                       unsigned char g, unsigned char b, unsigned char a, float anchorX, float anchorY)
            : Label(&fps, font, fontSize, x, y, r, g, b, a, anchorX, anchorY, false) {
        while (!frame_time.empty()) {
            frame_time.pop();
        }
    }

    void FPSLabel::Draw() const {
        double current_time = al_get_time();
        frame_time.emplace(current_time);
        while (frame_time.front() < current_time - 1.0) {
            frame_time.pop();
        }
        fps = std::to_string(frame_time.size());

        Label::Draw();
    }
}