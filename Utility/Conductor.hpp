#ifndef INC_2024_I2P2_TOWERDEFENSE_CONDUCTOR_HPP
#define INC_2024_I2P2_TOWERDEFENSE_CONDUCTOR_HPP

#include <vector>
#include <ctime>
#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"

class Conductor {
public:
    double bpm;
    double length_per_beat_in_seconds;
    double offset_in_seconds;
    double song_position;
    double startTime;

    Conductor();

    void update();

    void init(double bpm, double offset);
};


#endif //INC_2024_I2P2_TOWERDEFENSE_CONDUCTOR_HPP
