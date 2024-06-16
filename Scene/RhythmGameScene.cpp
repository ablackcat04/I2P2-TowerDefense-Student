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
        last_hit_time[i] = -10000;
        last_judgement[i] = Judgement::missed;
        last_pressed_time[i] = -10000;
    }
    combo = 0;
    current_judgement = "";
    notes.clear();
    while (!frame_time.empty()) {
        frame_time.pop();
    }
    allperfect = true;
    fullcombo = true;

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
    int has_note[lanes];
    float start_time;
    std::ifstream fin(filename);
    while(fin >> has_note[0] >> has_note[1] >> has_note[2] >> has_note[3] >> start_time){
        for(int i=0;i < lanes;i++){
            if(has_note[i]==1){//短條
                notes.emplace_back(Note(i, start_time, &red,false,10));
                //endtime=start_time*conductor.crotchet+10;
            }
            else if(has_note[i]>1){//長條
                notes.emplace_back(Note(i, start_time, &red,true,100*(has_note[i]-1)));
                //endtime=start_time*conductor.crotchet+10;
            }
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
        if (n->active && conductor.song_position - n->start_time * conductor.crotchet - 1 > 0.1) {
            allperfect= false;
            fullcombo = false;
            n = notes.erase(n);
            current_judgement = "Missed";
            --n;
            combo = 0;
        }
    }
    if(notes.empty()){
        if(allperfect){
            AddNewObject(new Engine::Label("Allperfect", "pirulen.ttf", 100, 804, 400, 255, 0, 0, 255, 0.5, 0.5));
        } else if(fullcombo){
            AddNewObject(new Engine::Label("Fullcombo", "pirulen.ttf", 100, 804, 400, 255, 0, 0, 255, 0.5, 0.5));
        }
    }
    //while(1) if(al_get_time()>=endtime) Terminate();
}

void RhythmGameScene::Draw() const {
    frame_time.emplace(conductor.song_position);
    while (frame_time.front() <= conductor.song_position - 1) {
        frame_time.pop();
    }
    fps = std::to_string(frame_time.size());

    IScene::Draw();

    for (int i = 0; i < lanes; ++i) {   // draw the pressed visual feed-back
        float t = conductor.song_position - last_pressed_time[i];
        if (t < 0) {
            continue;
        }
        if (t <= pressed_fx_attack) {
            al_draw_filled_rectangle(402 * i, 0, 402*(i+1), 700, al_map_rgba(255, 255, 255, pressed_fx_max_brightness*(t/pressed_fx_attack)));
        } else if (t <= pressed_fx_decay + pressed_fx_attack) {
            al_draw_filled_rectangle(402 * i, 0, 402*(i+1), 700, al_map_rgba(255, 255, 255, pressed_fx_max_brightness*(1 - (t-pressed_fx_attack)/pressed_fx_decay)));
        }
    }

    for (auto note : notes) {
        if (note.active) {
            note.render();
        } else {
            break;
        }
    }

    for (int i = 0; i < lanes; ++i) {   // draw the hit visual feed-back
        float t = conductor.song_position - last_hit_time[i];
        if (t < 0.1) {
            al_draw_rectangle(402 * i + 100 - (100)*(t*10), 675, 402 * (i + 1) - 100 + (100)*(t*10), 725, (last_judgement[i] == Judgement::perfect ? cyan : yellow), 100*(t+0.05));
        }
    }
}

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
        al_draw_filled_rectangle(402*x, y, 402*x + size, y + length, *note_color);
    }
}

void RhythmGameScene::OnKeyDown(int keyCode) {
    int lane = -1;
    for (int i = 0; i < lanes; ++i) {
        if (lane_key[i] == keyCode) {
            lane = i;
            last_pressed_time[lane] = conductor.song_position;
        }
    }
    if (lane == -1) {
        return;
    }

    for (auto n = notes.begin(); n != notes.end(); ++n) {
        float t = conductor.song_position - n->start_time * conductor.crotchet - 1;
        if (n->x == lane&&n->active) {
            if (t > -0.5 && t < 0.5) {
                if(!n->ishold){
                    n = notes.erase(n);
                    --n;
                }
                if (t > -0.2 && t < 0.2) {
                    score += 100;
                    current_judgement = "Perfect";
                    last_judgement[lane] = Judgement::perfect;
                } else {
                    score += 50;
                    current_judgement = " Good";
                    last_judgement[lane] = Judgement::good;
                    allperfect= false;
                }
                ++combo;
                last_hit_time[lane] = conductor.song_position;
                break;
            }
        }
    }
}

void RhythmGameScene::OnKeyUp(int keyCode){
    int lane = -1;
    for (int i = 0; i < lanes; ++i) {
        if (lane_key[i] == keyCode) {
            lane = i;
            last_up_time[lane] = conductor.song_position;
        }
    }
    if (lane == -1) {
        return;
    }
    for (auto n = notes.begin(); n != notes.end(); ++n) {
        float addtime=(float)n->length/700.0;
        float t = conductor.song_position - n->start_time * conductor.crotchet - 1 - addtime;
        if (n->x == lane&&n->ishold&&n->active) {
            if (t > -0.5 && t < 0.5) {
                n = notes.erase(n);
                --n;
                if (t > -0.2 && t < 0.2) {
                    score += 100;
                    current_judgement = "Perfect";
                    last_judgement[lane] = Judgement::perfect;
                } else {
                    score += 50;
                    current_judgement = " Good";
                    last_judgement[lane] = Judgement::good;
                    allperfect= false;
                }
                ++combo;
                last_hit_time[lane] = conductor.song_position;
                break;
            }
        }
    }
}