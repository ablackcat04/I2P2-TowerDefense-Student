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
#include "RhythmGameScene.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "Engine/AudioHelper.hpp"

RhythmGameScene::RhythmGameScene() : backgroundMusic(nullptr), musicInstance(nullptr) , conductor(){

};

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



    test_pos_label = new Engine::Label(&test_text, "BoutiqueBitmap7x7_1.7.ttf", 48, 120, 120, 255, 255, 255, 255, 0.0, 0.0);
    AddRefObject(*test_pos_label);

    conductor.init(100, 0);
}
void RhythmGameScene::Terminate() {
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

void RhythmGameScene::Update(float deltaTime){
    conductor.update();
    test_text = std::to_string(conductor.songPosition);
//    for (auto& note : notes) {
//        note.update();
//    }
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
//void Note::update() {
//    y += 5; // 掉落速度
//}