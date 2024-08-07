#ifndef INC_2024_I2P2_TOWERDEFENSE_NOTE_HPP
#define INC_2024_I2P2_TOWERDEFENSE_NOTE_HPP

#include <allegro5/color.h>
#include "Conductor.hpp"

class Note {
public:
    int x_shift = 1600;
    int width = 300;
    int lane;
    float y;
    float start_time;
    bool is_active;
    bool is_hold;
    bool will_be_hit_by_cheat;
    int length;
    ALLEGRO_COLOR* note_color;
    ALLEGRO_COLOR* note_color2;

    Note(int startX,float start, ALLEGRO_COLOR* color, ALLEGRO_COLOR *color2,bool longa,int length_of_note) : lane(startX), y(0),
                                                                                                              start_time(start), is_active(false), note_color(color) , is_hold(longa), will_be_hit_by_cheat(false), length(length_of_note), note_color2(color2) {}
    void update(Conductor conductor);
    void render();
};

#endif //INC_2024_I2P2_TOWERDEFENSE_NOTE_HPP
