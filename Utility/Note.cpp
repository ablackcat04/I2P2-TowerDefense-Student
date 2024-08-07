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
        if(!is_hold)al_draw_filled_rectangle(x_shift + width / 4 * lane, y - length, x_shift + width / 4 * (lane + 1), y , *note_color);
        else{
            if (y-length+20 > 700 && y-length <= 700) {
                al_draw_filled_rectangle(x_shift + width / 4 * lane, y - length, x_shift + width / 4 * (lane + 1), 700 , *note_color);
            } else if (y-length+20 <= 700) {
                al_draw_filled_rectangle(x_shift + width / 4 * lane, y - length, x_shift + width / 4 * (lane + 1), y - length + 20 , *note_color);
            }


            if (y - 10 > 700 && y - length + 10 <= 700) {
                al_draw_filled_rectangle(x_shift + width / 4 * lane, y - length + 10, x_shift + width / 4 * (lane + 1), 700, *note_color2);
            } else if (y - 10 <= 700) {
                al_draw_filled_rectangle(x_shift + width / 4 * lane, y - length + 10, x_shift + width / 4 * (lane + 1), y - 10, *note_color2);
            }

            if (y > 700 && y - 20 <= 700) {
                al_draw_filled_rectangle(x_shift + width / 4 * lane, y - 20, x_shift + width / 4 * (lane + 1), 700 , *note_color);
            } else if (y <= 700) {
                al_draw_filled_rectangle(x_shift + width / 4 * lane, y - 20, x_shift + width / 4 * (lane + 1), y , *note_color);
            }
        }
    }
}