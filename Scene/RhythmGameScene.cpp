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
}

void RhythmGameScene::Initialize() {
    combo = 0;
    current_judgement = "";
    notes.clear();
    while (!frame_time.empty()) {
        frame_time.pop();
    }

    frame_rate = 0.f;
    score = 0;
    font = al_load_font("Resource/fonts/BoutiqueBitmap7x7_1.7.ttf", 40, 0);
    white = al_map_rgb(255,255,255);

    // Load background music
//    backgroundMusic = al_load_sample("resources/music/background.ogg");
//    if (!backgroundMusic) {
//        throw std::runtime_error("Failed to load background music.");
//    }
//
//    // Create a music instance
//    musicInstance = al_create_sample_instance(backgroundMusic);
//    if (!musicInstance) {
//        throw std::runtime_error("Failed to create music instance.");
//    }
//
//    // Attach the instance to the default mixer
//    al_attach_sample_instance_to_mixer(musicInstance, al_get_default_mixer());
//
//    // Play the music looped
//    al_set_sample_instance_playmode(musicInstance, ALLEGRO_PLAYMODE_ONCE);
//    al_play_sample_instance(musicInstance);
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
    /*ypos = new float*[notes.size()];
    Engine::RefImage* testimage;
    int k=0;
    for (auto& note : notes) {
        ypos[k]=&note.y;
        k++;
    }
    k=0;
    /*for (auto& note : notes){
        if(note.starttime<=conductor.songPosition){
            testimage=new Engine::RefImage("stage-select/Note.png", w/note.x,ypos[k] , 200, 200, 0.5, 0.5);
            AddRefObject(*testimage);
        }
        testimage=new Engine::RefImage("stage-select/Note.png", w/notes[k].x,ypos[k] , 200, 200, 0.5, 0.5);
        AddRefObject(*testimage);
        k++;
    }*/

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
    // Stop and destroy the music instance
//    if (musicInstance) {
//        al_stop_sample_instance(musicInstance);
//        al_destroy_sample_instance(musicInstance);
//        musicInstance = nullptr;
//    }
//
//    // Destroy the background music sample
//    if (backgroundMusic) {
//        al_destroy_sample(backgroundMusic);
//        backgroundMusic = nullptr;
//    }
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

//    for (auto& note : notes) {
//        note.update(conductor);
//        if (note.active && conductor.songPosition - note.starttime * conductor.crotchet-1 > 0.1) {
//            note.destroy = true;
//        }
//    }
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
                } else {
                    score += 50;
                    current_judgement = " Good";
                }
                ++combo;
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
                } else {
                    score += 50;
                    current_judgement = " Good";
                }
                ++combo;
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
                } else {
                    score += 50;
                    current_judgement = " Good";
                }
                ++combo;
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
                } else {
                    score += 50;
                    current_judgement = " Good";
                }
                ++combo;
                break;
            }
        }
    }
}