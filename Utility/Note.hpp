#ifndef INC_2024_I2P2_TOWERDEFENSE_NOTE_HPP
#define INC_2024_I2P2_TOWERDEFENSE_NOTE_HPP

#include <allegro5/color.h>
#include "Conductor.hpp"

class Note {
public:
    int x_shift_in_pixel = 1600;
    int width_in_pixel = 300;
    int height_in_pixel;
    int lane;
    float y;
    float start_time;
    bool is_active;
    bool is_hold;
    bool will_note_be_hit_by_cheat;
    ALLEGRO_COLOR* note_color;
    ALLEGRO_COLOR* hold_color;

    Note(int startX, float start, ALLEGRO_COLOR* color, ALLEGRO_COLOR *color2, bool is_note_hold, int length_of_note)
    : lane(startX), y(0), start_time(start), is_active(false), note_color(color) , is_hold(is_note_hold),
        will_note_be_hit_by_cheat(false), height_in_pixel(length_of_note), hold_color(color2) {}

    void update(Conductor conductor);
    void render();
};

#endif //INC_2024_I2P2_TOWERDEFENSE_NOTE_HPP
