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
using namespace std;

class Note {
public:
    float y;    // 音符的 y 位置
    int x;// 音符的賽道1-4
    float size;    // 音符的大小
    float starttime;
    bool active;
    bool destroy;

    Note(int startX,float start) : x(startX), y(0), size(390),starttime(start),active(false), destroy(false) {}// 构造函数初始化音符的起始位置
    void update(Conductor conduc) ;// 更新方法，用于更新音符的位置
    void render();// 渲染方法，用于在屏幕上绘制音符


};

class RhythmGameScene final : public Engine::IScene{
private:
    int notesnum;
    vector<Note> notes; // 存储音符的容器
    ALLEGRO_SAMPLE* backgroundMusic;
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;
    Conductor conductor;

    Engine::Label* test_pos_label;
    std::string test_text;
    mutable std::string fps;
    float** ypos;//存儲音符y位置的指標的容器

    ALLEGRO_FONT* font;
    ALLEGRO_COLOR white;

    int score;

    float frame_rate;
    mutable std::queue<float> frame_time;

public:
    explicit RhythmGameScene();
    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void readnotes(int songID);//讀譜面
    void Draw() const override;
    void OnKeyDown(int keyCode) override;
};

#endif //INC_2024_I2P2_TOWERDEFENSE_RHYTHMGAMESCENE_HPP
