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
    int x;      // lane of the note (0 ~ 3)
    float y;    // the y position of the note
    float size;    // size of the note(x)
    float start_time;
    bool active;
    ALLEGRO_COLOR* note_color;

    Note(int startX,float start, ALLEGRO_COLOR* color) : x(startX), y(0), size(390), start_time(start), active(false), note_color(color) {}
    void update(Conductor conductor);
    void render();
};

class RhythmGameScene final : public Engine::IScene{
private:
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

    mutable std::queue<float> frame_time;

    float last_hit_time[lanes];
    float last_pressed_time[lanes];
    enum class Judgement {
        perfect, good, missed
    };
    Judgement last_judgement[lanes];

    int lane_key[lanes] = {ALLEGRO_KEY_D, ALLEGRO_KEY_F, ALLEGRO_KEY_J, ALLEGRO_KEY_K};

public:
    explicit RhythmGameScene();
    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void ReadNotes(int songID);
    void Draw() const override;
    void OnKeyDown(int keyCode) override;
};

#endif //INC_2024_I2P2_TOWERDEFENSE_RHYTHMGAMESCENE_HPP
