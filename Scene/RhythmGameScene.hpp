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
#include "Utility/Note.hpp"
using namespace std;

class RhythmGameScene final : public Engine::IScene {
private:
    int x_shift = 1600;
    int wid = 300;

    static const int lanes = 4;
    vector<Note> notes;
    ALLEGRO_SAMPLE* backgroundMusic;
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;
    Conductor conductor;

    int score;
    Engine::Label* score_label;
    std::string score_text;

    Engine::Label* fps_label;
    mutable std::string fps;

    Engine::Label* judgement_label;
    std::string current_judgement;


    int combo;
    Engine::Label* combo_label;
    std::string combo_text;

    ALLEGRO_FONT* font;

    ALLEGRO_COLOR cyan;
    ALLEGRO_COLOR yellow;
    ALLEGRO_COLOR red;
    ALLEGRO_COLOR blue;

    mutable std::queue<float> frame_time;

    float last_hit_time[lanes];
    float last_pressed_time[lanes];
    float last_up_time[lanes];
    enum class Judgement {
        perfect, good, missed
    };
    Judgement last_judgement[lanes];
    bool allperfect;
    bool fullcombo;
    float endtime;

    int lane_key[lanes] = {ALLEGRO_KEY_D, ALLEGRO_KEY_F, ALLEGRO_KEY_J, ALLEGRO_KEY_K};

    static const int pressed_fx_max_brightness = 16;
    constexpr static const float pressed_fx_attack = 0.02;  // in sec
    constexpr static const float pressed_fx_decay = 0.4;    // in sec

public:
    explicit RhythmGameScene();
    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void ReadNotes(int songID);
    void Draw() const override;
    void OnKeyDown(int keyCode) override;
    void OnKeyUp(int keyCode) override;
};

#endif //INC_2024_I2P2_TOWERDEFENSE_RHYTHMGAMESCENE_HPP
