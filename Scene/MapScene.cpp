//
#include <allegro5/allegro_audio.h>
#include <functional>
#include <memory>
#include <string>

#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "MapScene.hpp"
#include "WinScene.hpp"
#include "PlotScene.hpp"

void MapScene::Initialize() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;

    Engine::ImageButton *btn;
    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, halfH * 3 / 2 - 50,
                                  400, 100);
    btn->SetOnClickCallback(std::bind(&MapScene::BackOnClick, this, 1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Back", "pirulen.ttf", 48, halfW, halfH * 3 / 2, 0, 0, 0, 255, 0.5, 0.5));

    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW/2 - 150, halfH / 2 - 50, 200,
                                  200);
    btn->SetOnClickCallback(std::bind(&MapScene::PlayOnClick, this, 1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("1", "pirulen.ttf", 96, halfW/2-50, halfH / 2+50, 0, 0, 0, 255, 0.5, 0.5));
if(cnt>1) {
    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW/2+300 , halfH / 2 -50, 200,
                                  200);
    btn->SetOnClickCallback(std::bind(&MapScene::PlayOnClick, this, 2));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("2", "pirulen.ttf", 96, halfW/2+400, halfH / 2 +50, 0, 0, 0, 255, 0.5, 0.5));
}
else {
    AddNewObject(new Engine::Image("map/lock.jpg", halfW/2+400, halfH/2+50, 200, 200, 0.5, 0.5));
}
    if (cnt>2) {
        btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW/2+750 , halfH / 2 -50, 200,
                                     200);
        btn->SetOnClickCallback(std::bind(&MapScene::PlayOnClick, this, 3));
        AddNewControlObject(btn);
        AddNewObject(new Engine::Label("3", "pirulen.ttf", 96, halfW/2+850, halfH / 2+50 , 0, 0, 0, 255, 0.5, 0.5));
    }
    else {
        AddNewObject(new Engine::Image("map/lock.jpg", halfW/2+800, halfH/2+50, 200, 200, 0.5, 0.5));
    }
    // Not safe if release resource while playing, however we only free while change scene, so it's fine.
    bgmInstance = AudioHelper::PlaySample("select.ogg", true, AudioHelper::BGMVolume);
}


void MapScene::Terminate() {
    AudioHelper::StopSample(bgmInstance);
    bgmInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
    IScene::Terminate();
}

void MapScene::BackOnClick(int stage) {
    Engine::GameEngine::GetInstance().ChangeScene("stage-select");
}

void MapScene::PlayOnClick(int stage) {
//    PlayScene *scene = dynamic_cast<PlayScene *>(Engine::GameEngine::GetInstance().GetScene("play"));
//    scene->MapId = stage;
//    SetLastStage(stage);
//    Engine::GameEngine::GetInstance().ChangeScene("play");

    PlotScene* scene = dynamic_cast<PlotScene*>(Engine::GameEngine::GetInstance().GetScene("plot-scene"));
    scene->SetPlotPathTo("Resource/plot/plot" + std::to_string(stage) + ".txt");
    scene->stage = stage;
    scene->SetNextSceneTo("play");
    SetLastStage(stage);
    Engine::GameEngine::GetInstance().ChangeScene("plot-scene");
}
void MapScene::IncCount() {
    if(cnt<=3)cnt++;
}

int MapScene::GetCount() {
    return cnt;
}
// Created by Prox64 on 2024/6/11.
//
