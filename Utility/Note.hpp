//
// Created by wuru8 on 2024/6/20.
//

#ifndef INC_2024_I2P2_TOWERDEFENSE_NOTE_HPP
#define INC_2024_I2P2_TOWERDEFENSE_NOTE_HPP


#include <allegro5/color.h>
#include "Conductor.hpp"

class Note {
public:
    int x_shift = 1600;
    int wid = 300;
    int x;      // lane of the note (0 ~ 3)
    float y;    // the y position of the note
    float size;    // size of the note(x)
    float start_time;
    bool active;
    bool ishold;    //484長條
    int length;
    ALLEGRO_COLOR* note_color;
    ALLEGRO_COLOR* note_color2;

    Note(int startX,float start, ALLEGRO_COLOR* color, ALLEGRO_COLOR *color2,bool longa,int leng) : x(startX), y(0), size(390),
        start_time(start), active(false), note_color(color) ,ishold(longa),length(leng), note_color2(color2) {}
    void update(Conductor conductor);
    void render();
};

#endif //INC_2024_I2P2_TOWERDEFENSE_NOTE_HPP
