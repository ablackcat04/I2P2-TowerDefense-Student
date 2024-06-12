//
// Created by wuru8 on 2024/6/12.
//

#include "Conductor.hpp"
#include "Engine/LOG.hpp"

void Conductor::update() {
    double currentTime = al_get_time();
    songPosition = (currentTime - startTime) - offset;
}


Conductor::Conductor() {
    Engine::LOG(Engine::INFO) << "Conductor Created";
}

void Conductor::init(double bpm, double offset) {
    Engine::LOG(Engine::INFO) << "Conductor Initialized";
    crotchet = (double)60.0 / bpm;
    songPosition = 0.0f;
    startTime = al_get_time();
}