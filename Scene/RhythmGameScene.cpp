//
// Created by yuwei on 2024/6/11.
//
#include <queue>
#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>
#include "RhythmGameScene.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "Engine/AudioHelper.hpp"
#include "UI/Component/ImageButton.hpp"

using namespace std;

RhythmGameScene::RhythmGameScene() : backgroundMusic(nullptr), bgmInstance(nullptr) , conductor(), notesnum(0){
    white = al_map_rgb(255,255,255);
    cyan = al_map_rgb(0, 220, 220);
    yellow = al_map_rgb(220, 220, 0);
}

void RhythmGameScene::Initialize() {
    for (int i = 0; i < 4; ++i) {
        last_hit_time[i] = -1000;
        last_judgement[i] = Judgement::missed;
    }
    combo = 0;
    current_judgement = "";
    notes.clear();
    while (!frame_time.empty()) {
        frame_time.pop();
    }

    frame_rate = 0.f;
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
    readnotes(1);

    auto* a = new Engine::Label(&test_text, "BoutiqueBitmap7x7_1.7.ttf", 40, 0, 0, 255, 255, 255, 255);
    AddRefObject(*a);
    auto* b = new Engine::Label(&fps, "BoutiqueBitmap7x7_1.7.ttf", 40, 1400, 0, 255, 255, 255, 255);
    AddRefObject(*b);
    auto* c = new Engine::Label(&current_judgement, "BoutiqueBitmap7x7_1.7.ttf", 64, 700, 490, 255, 255, 255, 255, 1, 1);
    AddRefObject(*c);
    auto* d = new Engine::Label(&combo_text, "BoutiqueBitmap7x7_1.7.ttf", 64, 730, 400, 255, 255, 255, 255, 1, 1);
    AddRefObject(*d);

    Engine::ImageButton* btn;
    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", 1400, 50, 400, 50);
    btn->SetOnClickCallback([] { Engine::GameEngine::GetInstance().ChangeScene("stage-select");});
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Back", "pirulen.ttf", 24, 1500, 75, 0, 0, 0, 255, 0.5, 0.5));
}
void RhythmGameScene::Terminate() {
    AudioHelper::StopSample(bgmInstance);
    bgmInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
    delete[] ypos;
}

void RhythmGameScene::readnotes(int songID){
    std::string filename = std::string("Resource/song") + std::to_string(songID) ;
    bool hasnote[4];
    float starttime;
    std::ifstream fin(filename);
    while(fin >> hasnote[0] >> hasnote[1] >> hasnote[2] >> hasnote[3] >> starttime){
        for(int i=0;i<4;i++){
            if(hasnote[i]) notes.emplace_back(Note(i,starttime));
        }
    }
}

void RhythmGameScene::Update(float deltaTime){
    combo_text = std::to_string(combo);
    conductor.update();
    test_text = std::to_string(score);

    auto ite = notes.begin();
    while (ite != notes.end()) {
        if (ite->destroy) {
            ite = notes.erase(ite);
            current_judgement = "Missed";
        } else {
            ++ite;
        }
    }

    for (auto n = notes.begin(); n != notes.end(); ++n) {
        n->update(conductor);
        if (n->active && conductor.songPosition - n->starttime * conductor.crotchet-1 > 0.1) {
            n = notes.erase(n);
            --n;
            combo = 0;
        }
    }
}

void RhythmGameScene::Draw() const {
    IScene::Draw();
    for (auto note : notes) {
        if (note.active) {
            note.render();
        } else {
            break;
        }
    }

    frame_time.emplace(conductor.songPosition);
    while (frame_time.front() <= conductor.songPosition-1) {
        frame_time.pop();
    }
    fps = std::to_string(frame_time.size());

    for (int i = 0; i < 4; ++i) {
        float t = conductor.songPosition - last_hit_time[i];
        if (t < 0.1) {
            al_draw_rectangle(402 * i + 100 - (100)*(t*10), 675, 402 * (i + 1) - 100 + (100)*(t*10), 725, (last_judgement[i] == Judgement::perfect ? cyan : yellow), 100*(t+0.05));
        }
    }
}


void Note::update(Conductor conduc) {
    if (!active && conduc.songPosition >= starttime * conduc.crotchet) {
        active = true;
    }
    if (active) {
        y = 700*(conduc.songPosition - starttime * conduc.crotchet); // 位置改变
    }
}

void Note::render() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    if (active) {
        al_draw_filled_rectangle(402*x, y, 402*x + size, y + 10, al_map_rgb(255, 0, 0));
    }
}

void RhythmGameScene::OnKeyDown(int keyCode) {
    if (keyCode == ALLEGRO_KEY_D) {
        for (auto n = notes.begin(); n != notes.end(); ++n) {
            float t = conductor.songPosition - n->starttime * conductor.crotchet-1;
            if (n->x == 0 && (t > -0.1 && t < 0.1)) {
                n = notes.erase(n);
                --n;
                if (t > -0.05 && t < 0.05) {
                    score += 100;
                    current_judgement = "Perfect";
                    last_judgement[0] = Judgement::perfect;
                } else {
                    score += 50;
                    current_judgement = " Good";
                    last_judgement[0] = Judgement::good;
                }
                ++combo;
                last_hit_time[0] = conductor.songPosition;
                break;
            }
        }
    } else if (keyCode == ALLEGRO_KEY_F) {
        for (auto n = notes.begin(); n != notes.end(); ++n) {
            float t = conductor.songPosition - n->starttime * conductor.crotchet-1;
            if (n->x == 1 && (t > -0.1 && t < 0.1)) {
                n = notes.erase(n);
                --n;
                if (t > -0.05 && t < 0.05) {
                    score += 100;
                    current_judgement = "Perfect";
                    last_judgement[1] = Judgement::perfect;
                } else {
                    score += 50;
                    current_judgement = " Good";
                    last_judgement[1] = Judgement::good;
                }
                ++combo;
                last_hit_time[1] = conductor.songPosition;
                break;
            }
        }
    } else if (keyCode == ALLEGRO_KEY_J) {
        for (auto n = notes.begin(); n != notes.end(); ++n) {
            float t = conductor.songPosition - n->starttime * conductor.crotchet-1;
            if (n->x == 2 && (t > -0.1 && t < 0.1)) {
                n = notes.erase(n);
                --n;
                if (t > -0.05 && t < 0.05) {
                    score += 100;
                    current_judgement = "Perfect";
                    last_judgement[2] = Judgement::perfect;
                } else {
                    score += 50;
                    current_judgement = " Good";
                    last_judgement[2] = Judgement::good;
                }
                ++combo;
                last_hit_time[2] = conductor.songPosition;
                break;
            }
        }
    } else if (keyCode == ALLEGRO_KEY_K) {
        for (auto n = notes.begin(); n != notes.end(); ++n) {
            float t = conductor.songPosition - n->starttime * conductor.crotchet-1;
            if (n->x == 3 && (t > -0.1 && t < 0.1)) {
                n = notes.erase(n);
                --n;
                if (t > -0.05 && t < 0.05) {
                    score += 100;
                    current_judgement = "Perfect";
                    last_judgement[3] = Judgement::perfect;
                } else {
                    score += 50;
                    current_judgement = " Good";
                    last_judgement[3] = Judgement::good;
                }
                ++combo;
                last_hit_time[3] = conductor.songPosition;
                break;
            }
        }
    }
}