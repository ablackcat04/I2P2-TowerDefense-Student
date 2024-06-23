//
// Created by wuru8 on 2024/6/12.
//

#include "Conductor.hpp"
#include "Engine/LOG.hpp"

void Conductor::update() {
    double currentTime = al_get_time();
    song_position = (currentTime - startTime) - offset;
}

Conductor::Conductor() {
    Engine::LOG(Engine::INFO) << "Conductor Created";
    bpm=160;
    offset=0;
}

void Conductor::init(double bpm, double offset) {
    Engine::LOG(Engine::INFO) << "Conductor Initialized";
    this->bpm = bpm;
    crotchet = (double)60.0 / bpm;
    song_position = 0.0f;
    startTime = al_get_time();
}