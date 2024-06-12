//
// Created by wuru8 on 2024/6/12.
//

#ifndef INC_2024_I2P2_TOWERDEFENSE_CONDUCTOR_HPP
#define INC_2024_I2P2_TOWERDEFENSE_CONDUCTOR_HPP
#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <vector>
#include <ctime>
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"

class Conductor {
public:
    double bpm;
    double crotchet;   // length per beat, in seconds
    double offset;     // 歌曲的起始偏移量，用于补偿音频文件的开头空白
    double songPosition;
    double startTime; // 歌曲开始的时间

    // 构造函数初始化 bpm 和 offset，并计算 crotchet
    Conductor();

    // 更新方法，根据当前时间计算歌曲的位置
    void update();

    void init(double bpm, double offset);
};


#endif //INC_2024_I2P2_TOWERDEFENSE_CONDUCTOR_HPP
