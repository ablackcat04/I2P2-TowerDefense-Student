#include "Conductor.hpp"
#include "Engine/LOG.hpp"

void Conductor::update() {
    double currentTime = al_get_time();
    song_position = (currentTime - startTime) - offset_in_seconds;
}

Conductor::Conductor() {
    Engine::LOG(Engine::INFO) << "Conductor Created";
    bpm = 160;
    offset_in_seconds = 0;
}

void Conductor::init(double bpm, double offset) {
    Engine::LOG(Engine::INFO) << "Conductor Initialized";
    this->bpm = bpm;
    length_per_beat_in_seconds = (double)60.0 / bpm;
    song_position = 0.0f;
    startTime = al_get_time();
}