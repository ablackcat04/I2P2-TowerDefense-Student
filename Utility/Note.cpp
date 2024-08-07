#include "allegro5/allegro_primitives.h"
#include "Note.hpp"

void Note::update(Conductor conductor) {
    if (!is_active && conductor.song_position >= start_time * conductor.length_per_beat_in_seconds) {
        is_active = true;
    }
    if (is_active) {
        y = 700 * (conductor.song_position - start_time * conductor.length_per_beat_in_seconds);    // change the y position of the note
    }
}

void Note::render() {
    if (is_active) {
        if(!is_hold)al_draw_filled_rectangle(x_shift_in_pixel + width_in_pixel / 4 * lane, y - height_in_pixel, x_shift_in_pixel + width_in_pixel / 4 * (lane + 1), y , *note_color);
        else{
            if (y - height_in_pixel + 20 > 700 && y - height_in_pixel <= 700) {
                al_draw_filled_rectangle(x_shift_in_pixel + width_in_pixel / 4 * lane, y - height_in_pixel, x_shift_in_pixel + width_in_pixel / 4 * (lane + 1), 700 , *note_color);
            } else if (y - height_in_pixel + 20 <= 700) {
                al_draw_filled_rectangle(x_shift_in_pixel + width_in_pixel / 4 * lane, y - height_in_pixel, x_shift_in_pixel + width_in_pixel / 4 * (lane + 1), y - height_in_pixel + 20 , *note_color);
            }


            if (y - 10 > 700 && y - height_in_pixel + 10 <= 700) {
                al_draw_filled_rectangle(x_shift_in_pixel + width_in_pixel / 4 * lane, y - height_in_pixel + 10, x_shift_in_pixel + width_in_pixel / 4 * (lane + 1), 700, *hold_color);
            } else if (y - 10 <= 700) {
                al_draw_filled_rectangle(x_shift_in_pixel + width_in_pixel / 4 * lane, y - height_in_pixel + 10, x_shift_in_pixel + width_in_pixel / 4 * (lane + 1), y - 10, *hold_color);
            }

            if (y > 700 && y - 20 <= 700) {
                al_draw_filled_rectangle(x_shift_in_pixel + width_in_pixel / 4 * lane, y - 20, x_shift_in_pixel + width_in_pixel / 4 * (lane + 1), 700 , *note_color);
            } else if (y <= 700) {
                al_draw_filled_rectangle(x_shift_in_pixel + width_in_pixel / 4 * lane, y - 20, x_shift_in_pixel + width_in_pixel / 4 * (lane + 1), y , *note_color);
            }
        }
    }
}