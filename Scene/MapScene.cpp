#include <functional>
#include <memory>
#include <string>

#include <allegro5/allegro_audio.h>

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
    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, halfH*3 / 2 - 50,400, 100);
    btn->SetOnClickCallback(std::bind(&MapScene::BackOnClick, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Back", "pirulen.ttf", 48, halfW, halfH*3 / 2, 0, 0, 0, 255, 0.5, 0.5));

    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW/2 - 150, halfH/2 - 50, 200,200);
    btn->SetOnClickCallback(std::bind(&MapScene::StageOnClick, this, 1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("1", "pirulen.ttf", 96, halfW/2 - 50, halfH/2 + 50, 0, 0, 0, 255, 0.5, 0.5));

    if (maximum_opened_stage >= 2) {
        btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW/2+300 , halfH / 2 -50, 200,
                                      200);
        btn->SetOnClickCallback(std::bind(&MapScene::StageOnClick, this, 2));
        AddNewControlObject(btn);
        AddNewObject(new Engine::Label("2", "pirulen.ttf", 96, halfW/2 + 400, halfH/2 + 50, 0, 0, 0, 255, 0.5, 0.5));
    }
    else {
        AddNewObject(new Engine::Image("map/lock.jpg", halfW/2 + 400, halfH/2 + 50, 200, 200, 0.5, 0.5));
    }

    if (maximum_opened_stage >= 3) {
        btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW/2 + 750 , halfH/2 - 50, 200,200);
        btn->SetOnClickCallback(std::bind(&MapScene::StageOnClick, this, 3));
        AddNewControlObject(btn);
        AddNewObject(new Engine::Label("3", "pirulen.ttf", 96, halfW/2 + 850, halfH/2 + 50 , 0, 0, 0, 255, 0.5, 0.5));
    }
    else {
        AddNewObject(new Engine::Image("map/lock.jpg", halfW/2 + 800, halfH/2 + 50, 200, 200, 0.5, 0.5));
    }

    bgmInstance = AudioHelper::PlaySample("select.ogg", true, AudioHelper::BGMVolume);
}


void MapScene::Terminate() {
    AudioHelper::StopSample(bgmInstance);
    bgmInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
    IScene::Terminate();
}

void MapScene::BackOnClick() {
    Engine::GameEngine::GetInstance().ChangeScene("stage-select");
}

void MapScene::StageOnClick(int stage) {
    WinScene* win_scene = dynamic_cast<WinScene*>(Engine::GameEngine::GetInstance().GetScene("win-scene"));
    win_scene->SetLastStage(stage);

    PlotScene* plot_scene = dynamic_cast<PlotScene*>(Engine::GameEngine::GetInstance().GetScene("plot-scene"));
    plot_scene->SetPlotPathTo("Resource/plot/plot" + std::to_string(stage) + ".txt");
    plot_scene->stage = stage;
    plot_scene->SetNextSceneTo("play-scene");

    Engine::GameEngine::GetInstance().ChangeScene("plot-scene");
}

void MapScene::UnlockStage(int map_id) {
    if (map_id == maximum_opened_stage && maximum_opened_stage <= 3) {
        ++maximum_opened_stage;
    }
}
