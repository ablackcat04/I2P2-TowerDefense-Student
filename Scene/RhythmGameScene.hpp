//
// Created by yuwei on 2024/6/11.
//

#ifndef INC_2024_I2P2_TOWERDEFENSE_RHYTHMGAMESCENE_HPP
#define INC_2024_I2P2_TOWERDEFENSE_RHYTHMGAMESCENE_HPP

#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include <allegro5/allegro_audio.h>
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include <allegro5/allegro_audio.h>
#include <memory>
#include "Engine/IScene.hpp"
#include "UI/Component/Label.hpp"
#include "UI/Component/Image.hpp"
#include <queue>
#include <vector>
#include <unordered_map>
#include "UI/Component/RefImage.hpp"
#include "Utility/Conductor.hpp"

//class Conductor {
//public:
//    float bpm;
//    float crotchet;   // length per beat, in seconds
//    float offset;     // 歌曲的起始偏移量，用于补偿音频文件的开头空白
//    float songPosition;
//    double startTime; // 歌曲开始的时间
//
//    // 构造函数初始化 bpm 和 offset，并计算 crotchet
//    Conductor(float bpm, float offset) : bpm(bpm), offset(offset) {
//        crotchet = 60.0f / bpm;
//        songPosition = 0.0f;
//        startTime = al_get_time();
//    }
//
//    // 更新方法，根据当前时间计算歌曲的位置
//    void update() {
//        double currentTime = al_get_time();
//        songPosition = (currentTime - startTime) - offset;
//    }
//};
//
//class Note {
//public:
//    float x, y;    // 音符的 x 和 y 位置
//    float size;    // 音符的大小
//
//    // 构造函数初始化音符的起始位置
//    Note(float startX) : x(startX), y(0), size(50) {}
//
//    // 更新方法，用于更新音符的位置
//    void update() ;
//
//    // 渲染方法，用于在屏幕上绘制音符
//    void render();
//};

class RhythmGameScene final : public Engine::IScene{
private:
    ALLEGRO_SAMPLE* backgroundMusic;
    ALLEGRO_SAMPLE_INSTANCE* musicInstance;
    Conductor conductor;
    //std::vector<Note> notes; // 存储音符的容器

    Engine::Label* test_pos_label;
    std::string test_text;

public:
    explicit RhythmGameScene();
    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    //void Draw() const override;
    //void OnKeyDown(int keyCode) override;
};

#endif //INC_2024_I2P2_TOWERDEFENSE_RHYTHMGAMESCENE_HPP
