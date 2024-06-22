#include <allegro5/allegro_audio.h>
#include <functional>
#include <memory>
#include <string>

#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "PlayScene.hpp"
#include "StageSelectScene.hpp"
#include "WinScene.hpp"
#include "PlotScene.hpp"

void StageSelectScene::Initialize() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;

    Engine::ImageButton* btn;
    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, halfH * 3 / 2 - 50, 400, 100);
    btn->SetOnClickCallback(std::bind(&StageSelectScene::BackOnClick, this, 1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Back", "pirulen.ttf", 48, halfW, halfH * 3 / 2, 0, 0, 0, 255, 0.5, 0.5));

//    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, halfH / 2 - 50, 400, 100);
//    btn->SetOnClickCallback(std::bind(&StageSelectScene::PlayOnClick, this, 1));
//    AddNewControlObject(btn);
//    AddNewObject(new Engine::Label("Stage 1", "pirulen.ttf", 48, halfW, halfH / 2, 0, 0, 0, 255, 0.5, 0.5));
//
//    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, halfH /2 + 100, 400, 100);
//    btn->SetOnClickCallback(std::bind(&StageSelectScene::PlayOnClick, this, 2));
//    AddNewControlObject(btn);
//    AddNewObject(new Engine::Label("Stage 2", "pirulen.ttf", 48, halfW, halfH / 2 +150, 0, 0, 0, 255, 0.5, 0.5));

    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, halfH / 2 + 250, 400, 100);
    btn->SetOnClickCallback(std::bind(&StageSelectScene::ScoreboardOnClick, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Scoreboard", "pirulen.ttf", 36, halfW, halfH / 2 + 300, 0, 0, 0, 255, 0.5, 0.5));

//    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, halfH / 2 + 500, 400, 75);
//    btn->SetOnClickCallback(std::bind(&StageSelectScene::TestOnClick, this));
//    AddNewControlObject(btn);
//    AddNewObject(new Engine::Label("Test Plot", "pirulen.ttf", 36, halfW, halfH / 2 + 533, 0, 0, 0, 255, 0.5, 0.5));

    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, halfH / 2, 400, 150);
    btn->SetOnClickCallback(std::bind(&StageSelectScene::TestMapOnClick, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Story Mode", "pirulen.ttf", 36, halfW, halfH /2 + 75, 0, 0, 0, 255, 0.5, 0.5));

//    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", 100, 100, 200, 50);
//    btn->SetOnClickCallback(std::bind(&StageSelectScene::RhythmGameOnClick, this));
//    AddNewControlObject(btn);
//    AddNewObject(new Engine::Label("Rhythm Game", "pirulen.ttf", 20, 100, 100, 0, 0, 0, 255, 0.5, 0.5));


    // Not safe if release resource while playing, however we only free while change scene, so it's fine.
	bgmInstance = AudioHelper::PlaySample("select.ogg", true, AudioHelper::BGMVolume);
}

void StageSelectScene::Terminate() {
	AudioHelper::StopSample(bgmInstance);
	bgmInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
	IScene::Terminate();
}

void StageSelectScene::BackOnClick(int stage) {
    Engine::GameEngine::GetInstance().ChangeScene("start");
}

void StageSelectScene::PlayOnClick(int stage) {
    PlotScene* scene = dynamic_cast<PlotScene*>(Engine::GameEngine::GetInstance().GetScene("plot-scene"));
    scene->SetPlotPathTo("Resource/plot/plot" + std::to_string(stage) + ".txt");
    scene->stage = stage;
    scene->SetNextSceneTo("play");
    SetLastStage(stage);
    Engine::GameEngine::GetInstance().ChangeScene("plot-scene");
}

void StageSelectScene::ScoreboardOnClick() {
    Engine::GameEngine::GetInstance().ChangeScene("scoreboard-scene");
}
void StageSelectScene::TestOnClick() {
    Engine::GameEngine::GetInstance().ChangeScene("plot-scene");
}
void StageSelectScene::TestMapOnClick() {
    Engine::GameEngine::GetInstance().ChangeScene("map-scene");
}

void StageSelectScene::RhythmGameOnClick() {
    Engine::GameEngine::GetInstance().ChangeScene("rhythm-game-scene");
}