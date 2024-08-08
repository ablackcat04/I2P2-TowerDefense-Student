#include <functional>
#include <memory>

#include <allegro5/allegro_audio.h>

#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "PlayScene.hpp"
#include "SelectScene.hpp"
#include "Utility/UsefulConstants.hpp"

void SelectScene::Initialize() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;

    Engine::ImageButton* btn;
    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, halfH * 3 / 2 - 50, 400, 100);
    btn->SetOnClickCallback(std::bind(&SelectScene::BackOnClick, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Back", "pirulen.ttf", 48, halfW, halfH * 3 / 2, 0, 0, 0, 255, 0.5, 0.5));

    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, halfH / 2 + 250, 400, 100);
    btn->SetOnClickCallback(std::bind(&SelectScene::ScoreboardOnClick, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Scoreboard", "pirulen.ttf", 36, halfW, halfH / 2 + 300, 0, 0, 0, 255, 0.5, 0.5));

    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, halfH / 2, 400, 150);
    btn->SetOnClickCallback(std::bind(&SelectScene::StoryModeOnClick, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Story Mode", "pirulen.ttf", 36, halfW, halfH /2 + 75, 0, 0, 0, 255, 0.5, 0.5));

	bgmInstance = AudioHelper::PlaySample("select.ogg", true, AudioHelper::BGMVolume);
}

void SelectScene::Terminate() {
	AudioHelper::StopSample(bgmInstance);
	bgmInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
	IScene::Terminate();
}

void SelectScene::BackOnClick() {
    Engine::GameEngine::GetInstance().ChangeScene(SceneNames::start);
}

void SelectScene::ScoreboardOnClick() {
    Engine::GameEngine::GetInstance().ChangeScene(SceneNames::scoreboard);
}

void SelectScene::StoryModeOnClick() {
    Engine::GameEngine::GetInstance().ChangeScene(SceneNames::map);
}
