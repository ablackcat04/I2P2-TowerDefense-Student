//
// Created by wuru8 on 2024/6/20.
//
#include "allegro5/allegro_primitives.h"
#include "Note.hpp"

void Note::update(Conductor conductor) {
    if (!active && conductor.song_position >= start_time * conductor.crotchet) {
        active = true;
    }
    if (active) {
        y = 700*(conductor.song_position - start_time * conductor.crotchet);    // change the y position of the note
    }
}

void Note::render() {
    if (active) {
        if(!ishold)al_draw_filled_rectangle(x_shift + wid/4*x, y-length, x_shift + wid/4*(x + 1), y , *note_color);
        else{
            if (y-length+20 > 700 && y-length <= 700) {
                al_draw_filled_rectangle(x_shift + wid/4*x, y-length, x_shift + wid/4*(x + 1), 700 , *note_color);
            } else if (y-length+20 <= 700) {
                al_draw_filled_rectangle(x_shift + wid/4*x, y-length, x_shift + wid/4*(x + 1), y-length+20 , *note_color);
            }


            if (y - 10 > 700 && y - length + 10 <= 700) {
                al_draw_filled_rectangle(x_shift + wid / 4 * x, y - length + 10, x_shift + wid / 4 * (x + 1), 700,*note_color2);
            } else if (y - 10 <= 700) {
                al_draw_filled_rectangle(x_shift + wid / 4 * x, y - length + 10, x_shift + wid / 4 * (x + 1), y - 10,*note_color2);
            }

            if (y > 700 && y - 20 <= 700) {
                al_draw_filled_rectangle(x_shift + wid/4*x, y-20, x_shift + wid/4*(x + 1), 700 , *note_color);
            } else if (y <= 700) {
                al_draw_filled_rectangle(x_shift + wid/4*x, y-20, x_shift + wid/4*(x + 1), y , *note_color);
            }
        }
    }
}