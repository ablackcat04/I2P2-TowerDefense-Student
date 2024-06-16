//
// Created by yuwei on 2024/6/11.
//
#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_primitives.h>
#include <vector>
#include <iostream>
#include <fstream>
#include "RhythmGameScene.hpp"
#include "Engine/AudioHelper.hpp"
#include "UI/Component/ImageButton.hpp"

using namespace std;

RhythmGameScene::RhythmGameScene() : backgroundMusic(nullptr), bgmInstance(nullptr) , conductor(){
    cyan = al_map_rgb(0, 220, 220);
    yellow = al_map_rgb(220, 220, 0);
    red = al_map_rgb(255, 0, 0);
}

void RhythmGameScene::Initialize() {
    for (int i = 0; i < lanes; ++i) {
        last_hit_time[i] = -1000;
        last_judgement[i] = Judgement::missed;
    }
    combo = 0;
    current_judgement = "";
    notes.clear();
    while (!frame_time.empty()) {
        frame_time.pop();
    }

    score = 0;
    font = al_load_font("Resource/fonts/BoutiqueBitmap7x7_1.7.ttf", 40, 0);

    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    bgmInstance = AudioHelper::PlaySample("rhythm_game_test_audio_bpm_160.ogg", true, AudioHelper::BGMVolume);
    conductor.init(100, 0);
    AddNewObject(new Engine::Image("stage-select/defineline.png", halfW, 700, 1608, 15, 0.5, 0.5));
    AddNewObject(new Engine::Image("stage-select/defineline.png", halfW/2, halfH, 15, 802, 0.5, 0.5));
    AddNewObject(new Engine::Image("stage-select/defineline.png", halfW, halfH, 15, 802, 0.5, 0.5));
    AddNewObject(new Engine::Image("stage-select/defineline.png", halfW*1.5, halfH, 15, 802, 0.5, 0.5));
    ReadNotes(1);

    score_label = new Engine::Label(&score_text, "BoutiqueBitmap7x7_1.7.ttf", 40, 0, 0, 255, 255, 255, 255);
    AddRefObject(*score_label);
    fps_label = new Engine::Label(&fps, "BoutiqueBitmap7x7_1.7.ttf", 40, 1400, 0, 255, 255, 255, 255);
    AddRefObject(*fps_label);
    judgement_label = new Engine::Label(&current_judgement, "BoutiqueBitmap7x7_1.7.ttf", 64, 700, 490, 255, 255, 255, 255, 1, 1);
    AddRefObject(*judgement_label);
    combo_label = new Engine::Label(&combo_text, "BoutiqueBitmap7x7_1.7.ttf", 64, 730, 400, 255, 255, 255, 255, 1, 1);
    AddRefObject(*combo_label);

    Engine::ImageButton* btn;
    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", 1400, 50, 400, 50);
    btn->SetOnClickCallback([] { Engine::GameEngine::GetInstance().ChangeScene("stage-select");});
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Back", "pirulen.ttf", 24, 1500, 75, 0, 0, 0, 255, 0.5, 0.5));
}

void RhythmGameScene::Terminate() {
    AudioHelper::StopSample(bgmInstance);
    bgmInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
    delete score_label;
    delete combo_label;
    delete judgement_label;
    delete fps_label;
}

void RhythmGameScene::ReadNotes(int songID){
    std::string filename = std::string("Resource/song") + std::to_string(songID) ;
    bool has_note[lanes];
    float start_time;
    std::ifstream fin(filename);
    while(fin >> has_note[0] >> has_note[1] >> has_note[2] >> has_note[3] >> start_time){
        for(int i=0;i < lanes;i++){
            if(has_note[i]) notes.emplace_back(Note(i, start_time, &red));
        }
    }
}

void RhythmGameScene::Update(float deltaTime){
    combo_text = std::to_string(combo);
    conductor.update();
    score_text = std::to_string(score);

    auto ite = notes.begin();

    for (auto n = notes.begin(); n != notes.end(); ++n) {
        n->update(conductor);
        if (n->active && conductor.songPosition - n->start_time * conductor.crotchet - 1 > 0.1) {
            n = notes.erase(n);
            current_judgement = "Missed";
            --n;
            combo = 0;
        }
    }
}

void RhythmGameScene::Draw() const {
    frame_time.emplace(conductor.songPosition);
    while (frame_time.front() <= conductor.songPosition-1) {
        frame_time.pop();
    }
    fps = std::to_string(frame_time.size());
    IScene::Draw();
    for (auto note : notes) {
        if (note.active) {
            note.render();
        } else {
            break;
        }
    }

    for (int i = 0; i < lanes; ++i) {
        float t = conductor.songPosition - last_hit_time[i];
        if (t < 0.1) {
            al_draw_rectangle(402 * i + 100 - (100)*(t*10), 675, 402 * (i + 1) - 100 + (100)*(t*10), 725, (last_judgement[i] == Judgement::perfect ? cyan : yellow), 100*(t+0.05));
        }
    }
}


void Note::update(Conductor conductor) {
    if (!active && conductor.songPosition >= start_time * conductor.crotchet) {
        active = true;
    }
    if (active) {
        y = 700*(conductor.songPosition - start_time * conductor.crotchet); // 位置改变
    }
}

void Note::render() {
    if (active) {
        al_draw_filled_rectangle(402*x, y, 402*x + size, y + 10, *note_color);
    }
}

void RhythmGameScene::OnKeyDown(int keyCode) {
    int lane = -1;
    for (int i = 0; i < lanes; ++i) {
        if (lane_key[i] == keyCode) {
            lane = i;
        }
    }
    if (lane == -1) {
        return;
    }

    for (auto n = notes.begin(); n != notes.end(); ++n) {
        float t = conductor.songPosition - n->start_time * conductor.crotchet - 1;
        if (n->x == lane && (t > -0.1 && t < 0.1)) {
            n = notes.erase(n);
            --n;
            if (t > -0.05 && t < 0.05) {
                score += 100;
                current_judgement = "Perfect";
                last_judgement[lane] = Judgement::perfect;
            } else {
                score += 50;
                current_judgement = " Good";
                last_judgement[lane] = Judgement::good;
            }
            ++combo;
            last_hit_time[lane] = conductor.songPosition;
            break;
        }
    }
}