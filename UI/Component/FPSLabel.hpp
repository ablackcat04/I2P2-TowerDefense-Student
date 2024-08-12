#ifndef INC_2024_I2P2_TOWERDEFENSE_FPSLABEL_HPP
#define INC_2024_I2P2_TOWERDEFENSE_FPSLABEL_HPP

#include <queue>
#include "Label.hpp"

namespace Engine{
    class FPSLabel : public Label{
    private:
        mutable std::queue<float> frame_time;
        mutable std::string fps;
    public:
        FPSLabel(const std::string &font, int fontSize, float x, float y, unsigned char r, unsigned char g, unsigned char b,
                 unsigned char a, float anchorX, float anchorY);

        void Draw() const;
    };
}


#endif //INC_2024_I2P2_TOWERDEFENSE_FPSLABEL_HPP
