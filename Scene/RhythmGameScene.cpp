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
    blue = al_map_rgb(0,0,255);
}

void RhythmGameScene::Initialize() {
    al_resize_display(Engine::GameEngine::GetInstance().GetDisplay(), 1900, 832);

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
    int halfW = (w - x_shift) / 2;
    int halfH = h / 2;
    //bgmInstance = AudioHelper::PlaySample("Beyond_Apocalypse.ogg", true, AudioHelper::BGMVolume);
    bgmInstance = AudioHelper::PlaySample("Monochize.ogg", true, AudioHelper::BGMVolume);
    conductor.init(100, 0);
    AddNewObject(new Engine::Image("stage-select/defineline.png", x_shift, 700, wid, 10, 0.0, 0.5));
    AddNewObject(new Engine::Image("stage-select/defineline.png", x_shift, halfH, 4, 1000, 0.0, 0.5));
    AddNewObject(new Engine::Image("stage-select/defineline.png", x_shift+wid/2, halfH, 4, 1000, 0.0, 0.5));
    AddNewObject(new Engine::Image("stage-select/defineline.png", x_shift+wid/4, halfH, 4, 1000, 0.0, 0.5));
    AddNewObject(new Engine::Image("stage-select/defineline.png", x_shift+wid*3/4, halfH, 4, 1000, 0.0, 0.5));
    ReadNotes(2);

    score_label = new Engine::Label(&score_text, "BoutiqueBitmap7x7_1.7.ttf", 40, x_shift, 0, 255, 255, 255, 255);
    AddRefObject(*score_label);
    fps_label = new Engine::Label(&fps, "BoutiqueBitmap7x7_1.7.ttf", 40, x_shift + wid - 50, 0, 255, 255, 255, 255);
    AddRefObject(*fps_label);
    judgement_label = new Engine::Label(&current_judgement, "BoutiqueBitmap7x7_1.7.ttf", 64, x_shift + halfW, 490, 255, 255, 255, 255, 1, 1);
    AddRefObject(*judgement_label);
    combo_label = new Engine::Label(&combo_text, "BoutiqueBitmap7x7_1.7.ttf", 64, x_shift + halfW, 400, 255, 255, 255, 255, 1, 1);
    AddRefObject(*combo_label);

    Engine::ImageButton* btn;
    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", w - 100, 50, 50, 50);
    btn->SetOnClickCallback([] { Engine::GameEngine::GetInstance().ChangeScene("stage-select");});
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Back", "pirulen.ttf", 24, w-100, 75, 0, 0, 0, 255, 0.5, 0.5));

}

void RhythmGameScene::Terminate() {
    AudioHelper::StopSample(bgmInstance);
    bgmInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
    IScene::Terminate();
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
                endtime=start_time*conductor.crotchet+5;
                notes.emplace_back(Note(i, start_time, &red, &blue,false,10));
            }
            else if(has_note[i]>1){//長條
                endtime=start_time*conductor.crotchet+5;
                notes.emplace_back(Note(i, start_time, &red, &blue,true,100*(has_note[i]-1)));
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
        float t;
        if(n->ishold){
            float addtime=(float)n->length/700.0;
            t = conductor.song_position - n->start_time * conductor.crotchet - 1 - addtime;
        }else t=conductor.song_position - n->start_time * conductor.crotchet - 1;
        if (t> 0.1) {
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
            AddNewObject(new Engine::Label("Allperfect", "pirulen.ttf", 100, (x_shift + wid)/2, 400, 255, 0, 0, 255, 0.5, 0.5));
        } else if(fullcombo){
            AddNewObject(new Engine::Label("Fullcombo", "pirulen.ttf", 100, (x_shift + wid)/2, 400, 255, 0, 0, 255, 0.5, 0.5));
        }
    }
    if(conductor.song_position>=endtime) Engine::GameEngine::GetInstance().ChangeScene("stage-select");;
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
            al_draw_filled_rectangle(x_shift + (wid/4 * i), 0, x_shift + wid/4*(i+1), 700, al_map_rgba(255, 255, 255, pressed_fx_max_brightness*(t/pressed_fx_attack)));
        } else if (t <= pressed_fx_decay + pressed_fx_attack) {
            al_draw_filled_rectangle(x_shift + wid/4 * i, 0, x_shift + wid/4*(i+1), 700, al_map_rgba(255, 255, 255, pressed_fx_max_brightness*(1 - (t-pressed_fx_attack)/pressed_fx_decay)));
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
            al_draw_rectangle(x_shift + wid/4 * i + 100 - (100)*(t*10), 675, x_shift + wid/4 * (i + 1) - 100 + (100)*(t*10), 725, (last_judgement[i] == Judgement::perfect ? cyan : yellow), 100*(t+0.05));
        }
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
            if (t > -0.1 && t < 0.1) {
                if(!n->ishold){
                    n = notes.erase(n);
                    --n;
                }
                if (t > -0.05 && t < 0.05) {
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
            if (t > -0.1 && t < 0.1) {
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
                    allperfect= false;
                }
                ++combo;
                last_up_time[lane] = conductor.song_position;
                break;
            }
        }
    }
}