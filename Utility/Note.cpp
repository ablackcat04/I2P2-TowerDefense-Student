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
        //if(ishold) al_draw_filled_rectangle(402*x, y, 402*x + size, y + 500, *note_color);
        //else al_draw_filled_rectangle(402*x, y, 402*x + size, y + 10, *note_color);
        if(!ishold)al_draw_filled_rectangle(x_shift + wid/4*x, y-length, x_shift + wid/4*(x + 1), y , *note_color);
        else{
            al_draw_filled_rectangle(x_shift + wid/4*x, y-length, x_shift + wid/4*(x + 1), y-length+20 , *note_color);
            al_draw_filled_rectangle(x_shift + wid/4*x, y-length+10, x_shift + wid/4*(x + 1), y-10 , *note_color2);
            al_draw_filled_rectangle(x_shift + wid/4*x, y-20, x_shift + wid/4*(x + 1), y , *note_color);
        }
    }
}