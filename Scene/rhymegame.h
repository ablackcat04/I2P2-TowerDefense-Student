//
// Created by yuwei on 2024/6/11.
//

#ifndef INC_2024_I2P2_TOWERDEFENSE_RHYMEGAME_H
#define INC_2024_I2P2_TOWERDEFENSE_RHYMEGAME_H

#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <list>
#include <memory>
#include <utility>
#include <vector>

#include "Engine/IScene.hpp"
#include "Engine/Point.hpp"

class rhymescene final : public Engine::IScene{
private:
    ALLEGRO_SAMPLE_ID bgmId;
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> deathBGMInstance;
protected:
public:
    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void Draw() const override;
    // void ModifyReadMapTiles();
};

class Conductor {
public:
    float bpm;        // 歌曲的每分钟节拍数 (beats per minute)
    float crotchet;   // 每个节拍的时长（以秒为单位）
    float offset;     // 歌曲的起始偏移量，用于补偿音频文件的开头空白
    float songPosition;  // 当前歌曲的位置（以秒为单位）
    double startTime; // 歌曲开始的时间

    // 构造函数初始化 bpm 和 offset，并计算 crotchet
    Conductor(float bpm, float offset) : bpm(bpm), offset(offset) {
        crotchet = 60.0f / bpm;
        songPosition = 0.0f;
        startTime = al_get_time();
    }

    // 更新方法，根据当前时间计算歌曲的位置
    void update() {
        double currentTime = al_get_time();
        songPosition = (currentTime - startTime) - offset;
    }
};

class Note {
public:
    float x, y;    // 音符的 x 和 y 位置
    float size;    // 音符的大小

    // 构造函数初始化音符的起始位置
    Note(float startX) : x(startX), y(0), size(50) {}

    // 更新方法，用于更新音符的位置
    void update() {
        y += 5; // 掉落速度
    }

    // 渲染方法，用于在屏幕上绘制音符
    void render() {
        al_draw_filled_rectangle(x, y, x + size, y + size, al_map_rgb(255, 0, 0));
    }
};



#endif //INC_2024_I2P2_TOWERDEFENSE_RHYMEGAME_H
