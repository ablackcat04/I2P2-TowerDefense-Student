#include <allegro5/allegro_audio.h>
#include <functional>
#include <memory>
#include <string>

#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "PlayScene.hpp"
#include "ScoreboardScene.hpp"
#include "Engine/LOG.hpp"
#include "UI/Component/Scoreboard.hpp"
#include "Utility/InternetHelper.hpp"

const int max_scene = 3;

Engine::Scoreboard* scb[max_scene][2];      //scene, money/life
Engine::ImageButton* money_btn;
Engine::ImageButton* life_btn;
unsigned int scene = 1;
unsigned int money_or_life = 0;     //money: 0, life: 1

void AddScene(int a) {
    scene += a;
    if (scene > max_scene) {
        scene = max_scene;
    } else if (scene < 1) {
        scene = 1;
    }
}

Engine::Scoreboard* GetCurrentScoreboard() {
    return scb[scene-1][money_or_life];
}

void OnlyShowCurrentScoreboard(Engine::Scoreboard* current) {
    for (int i = 0; i <= max_scene-1; ++i) {
        for (int j = 0; j <= max_scene-1; ++j) {
            scb[i][j]->MakeNotShow();
        }
    }
    current->MakeShow();
}

void SetMoney() {
    money_or_life = 0;
}

void SetLife() {
    money_or_life = 1;
}

void ScoreboardScene::Initialize() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;

    if (InternetHelper::uploadFile(InternetHelper::upload_php_position, "Resource/scoreboard/local_test.txt")) {
        Engine::LOG(Engine::INFO) << "Upload Test Success!";
    } else {
        Engine::LOG(Engine::ERROR) << "Upload Test Failed!";
    }

    for (int i = 1; i <= max_scene; ++i) {
        if (InternetHelper::downloadFile(InternetHelper::server_ip +  "/I2P_project/scoreboard/stage" + std::to_string(i) + "_moneyLeft_scoreboard.txt",
                                         "Resource/scoreboard/online/stage" + std::to_string(i) + "_moneyLeft_scoreboard.txt")) {
            Engine::LOG(Engine::INFO) << "Load Successfully";
            online_avaliable[i-1][0] = true;
        } else {
            Engine::LOG(Engine::INFO) << "Failed to Load File";
            online_avaliable[i-1][0] = false;
        }

        if (InternetHelper::downloadFile(InternetHelper::server_ip +  "/I2P_project/scoreboard/stage" + std::to_string(i) + "_lifeLeft_scoreboard.txt",
                                         "Resource/scoreboard/online/stage" + std::to_string(i) + "_lifeLeft_scoreboard.txt")) {
            Engine::LOG(Engine::INFO) << "Load Successfully";
            online_avaliable[i-1][1] = true;
        } else {
            Engine::LOG(Engine::INFO) << "Failed to Load File";
            online_avaliable[i-1][1] = false;
        }
    }

    //AddNewObject(new Engine::Label("Scoreboard", "pirulen.ttf", 48, halfW, 50, 255, 255, 255, 255, 0.5, 0.5));

    Lscb = new Engine::Label(&title_name, "pirulen.ttf", 48, 400, 50, 255, 255, 255, 255, 0.5, 0.5);

    AddRefObject(*Lscb);

    Engine::ImageButton* btn;
    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, halfH * 7 / 4 - 50, 400, 100);
    btn->SetOnClickCallback([this] { BackOnClick(1); });
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Back", "pirulen.ttf", 48, halfW, halfH * 7 / 4, 0, 0, 0, 255, 0.5, 0.5));

    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW * 1/3 - 200, halfH * 7 / 4 - 50 , 400, 100);
    btn->SetOnClickCallback([this] { PrevOnClick(); });
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Prev Page", "pirulen.ttf", 48, halfW * 1/3, halfH * 7 / 4, 0, 0, 0, 255, 0.5, 0.5));

    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW * 5/3 - 200, halfH * 7 / 4 - 50, 400, 100);
    btn->SetOnClickCallback([] { NextOnClick(); });
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Next Page", "pirulen.ttf", 48, halfW * 5/3, halfH * 7 / 4, 0, 0, 0, 255, 0.5, 0.5));

    btn = new Engine::ImageButton("stage-select/arrow_left.png", "stage-select/arrow_left_hovered.png", 1450, 20, 64, 64);
    btn->SetOnClickCallback([] { AddScene(1); });
    AddNewControlObject(btn);

    btn = new Engine::ImageButton("stage-select/arrow_right.png", "stage-select/arrow_right_hovered.png", 1350, 20, 64, 64);
    btn->SetOnClickCallback([] { AddScene(-1); });
    AddNewControlObject(btn);


    money_btn = new Engine::ImageButton("stage-select/money.png", "stage-select/money_hovered.png", 1350, 120, 64, 64);
    money_btn->SetOnClickCallback(&SetMoney);
    AddRefControlObject(*money_btn);

    life_btn = new Engine::ImageButton("stage-select/heart.png", "stage-select/heart_hovered.png", 1450, 120, 64, 64);
    life_btn->SetOnClickCallback(&SetLife);
    AddRefControlObject(*life_btn);


//    //scoreboard component init
    std::string ss = "Resource/scoreboard/scoreboard.txt";

    for (int i = 1; i <= max_scene; i++) {
        if (online_avaliable[i-1][0]) {
            ss = (std::string)"Resource/scoreboard/online/stage" + std::to_string(i) + "_moneyLeft_scoreboard.txt";
        } else {
            ss = (std::string) "Resource/scoreboard/stage" + std::to_string(i) + "_moneyLeft_scoreboard.txt";
        }
        scb[i-1][0] = new Engine::Scoreboard(ss, 150, 100, 220, 220, 55);
        AddRefObject(*scb[i-1][0]);
        AddRefControl(*scb[i-1][0]);

        if (online_avaliable[i-1][1]) {
            ss = (std::string)"Resource/scoreboard/online/stage" + std::to_string(i) + "_lifeLeft_scoreboard.txt";
        } else {
            ss = (std::string)"Resource/scoreboard/stage" + std::to_string(i) + "_lifeLeft_scoreboard.txt";
        }
        scb[i-1][1] = new Engine::Scoreboard(ss, 150, 100, 255, 200, 200);
        AddRefObject(*scb[i-1][1]);
        AddRefControl(*scb[i-1][1]);
    }

    Engine::LOG(Engine::INFO) << "All Scoreboard Loads Successfully";

    // Not safe if release resource while playing, however we only free while change scene, so it's fine.
    bgmInstance = AudioHelper::PlaySample("select.ogg", true, AudioHelper::BGMVolume);
}

void ScoreboardScene::Update(float deltaTime) {
    title_name = (std::string)"Scoreboard - " + "stage" + (char)(scene + '0')/* + (money_or_life == 0 ? ":Money" : ":Life")*/;
    OnlyShowCurrentScoreboard(GetCurrentScoreboard());
}

void ScoreboardScene::Terminate() {
    AudioHelper::StopSample(bgmInstance);
    bgmInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
    for (int i = 0; i <= max_scene-1; i++) {
        delete scb[i][0];
        delete scb[i][1];
    }
    delete Lscb;
    IScene::Terminate();
}
void ScoreboardScene::BackOnClick(int stage) {
    Engine::GameEngine::GetInstance().ChangeScene("stage-select");
}
void ScoreboardScene::PrevOnClick() {
    Engine::LOG(Engine::INFO) << "Prev Button Clicked!";
    GetCurrentScoreboard()->PrevPage();
}
void ScoreboardScene::NextOnClick() {
    Engine::LOG(Engine::INFO) << "Next Button Clicked!";
    GetCurrentScoreboard()->NextPage();
}
