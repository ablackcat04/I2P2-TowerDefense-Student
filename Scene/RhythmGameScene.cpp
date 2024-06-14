//
// Created by yuwei on 2024/6/11.
//
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
using namespace std;

RhythmGameScene::RhythmGameScene() : backgroundMusic(nullptr), bgmInstance(nullptr) , conductor(), notesnum(0){
}

void RhythmGameScene::Initialize() {
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
    AddNewObject(new Engine::Image("stage-select/defineline.png", halfW, halfH+300, 1608, 15, 0.5, 0.5));
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
    conductor.update();
    test_text = std::to_string(conductor.songPosition);
    beattext = std::to_string(conductor.songPosition/conductor.crotchet);
    for (auto& note : notes) {
        note.update(conductor);
    }
}

void RhythmGameScene::Draw() const {
    IScene::Draw();
    for (auto note : notes) {
        note.render();
    }
}


//void RhythmGameScene::Draw() const{
//    IScene::Draw();
//    for (auto note1 : notes) {
//        note1.render();
//    }
//}
//void RhythmGameScene::OnKeyDown(int keyCode) {
//    Engine::IScene::OnKeyDown(keyCode);
//    if (keyCode == ALLEGRO_KEY_M) {
//        if (al_get_sample_instance_playing(musicInstance)) {
//            al_stop_sample_instance(musicInstance);
//        } else {
//            al_play_sample_instance(musicInstance);
//        }
//    }
//}
//
//void Note::render() {
//    al_draw_filled_rectangle(x, y, x + size, y + size, al_map_rgb(255, 0, 0));
//}
//
void Note::update(Conductor conduc) {
    if (conduc.songPosition >= starttime) {
        active = true;
    }
    if (active) {
        y += 5; // 位置改变
    }
}

void Note::render() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    if (active) {
        al_draw_filled_rectangle(402*x, y, 402*x + size, y + 150, al_map_rgb(255, 0, 0));
    }
}