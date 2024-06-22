#include <functional>
#include <string>

#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "UI/Component/Image.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "PlayScene.hpp"
#include "Engine/Point.hpp"
#include "WinScene.hpp"
#include "Engine/LOG.hpp"
#include "UI/Component/Label.hpp"
#include "UI/Component/Scoreboard.hpp"
#include "Scene/PlotScene.hpp"
#include "Utility/InternetHelper.hpp"

Engine::Label *Lwin;

static WinSceneInfo win_scene_info = {1, 0, 0};

void SetLastStage(int stage) {
    win_scene_info.last_stage = stage;
}

void SetLastGameInfo(int money, int life) {
    win_scene_info.money_left = money;
    win_scene_info.life_left = life;
}

void WinScene::Initialize() {
	ticks = 0;
	int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
	int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
	int halfW = w / 2;
	int halfH = h / 2;
	AddNewObject(new Engine::Image("win/benjamin-sad.png", halfW, halfH, 0, 0, 0.5, 0.5));
	AddNewObject(new Engine::Label("You Win!", "pirulen.ttf", 48, halfW, halfH / 4 -10, 255, 255, 255, 255, 0.5, 0.5));
	Engine::ImageButton* btn;
	btn = new Engine::ImageButton("win/dirt.png", "win/floor.png", halfW - 200, halfH * 7 / 4 - 50, 400, 100);
	btn->SetOnClickCallback(std::bind(&WinScene::ProceedOnClick, this, 2));
	AddNewControlObject(btn);
	AddNewObject(new Engine::Label("Proceed", "pirulen.ttf", 48, halfW, halfH * 7 / 4, 0, 0, 0, 255, 0.5, 0.5));
	bgmId = AudioHelper::PlayAudio("win.wav");

    Lwin = new Engine::Label(&username, "pirulen.ttf", 32, halfW-200, 150, 255, 255, 255, 255, 0.5, 0.5, true);

    AddRefObject(*Lwin);
}
void WinScene::Terminate() {
    Engine::LOG(Engine::INFO) << "Username: " << username;
    if (username == "") {
        username = "???";
    }

    std::string path;
    if (InternetHelper::downloadFile(InternetHelper::server_ip + "/I2P_project/scoreboard/stage" + std::to_string(win_scene_info.last_stage) + "_moneyLeft_scoreboard.txt",
                                     "Resource/scoreboard/online/stage" + std::to_string(win_scene_info.last_stage) + "_moneyLeft_scoreboard.txt")) {
        path = "Resource/scoreboard/online/stage" + std::to_string(win_scene_info.last_stage) + "_moneyLeft_scoreboard.txt";
        Engine::Scoreboard money_scoreboard(path, 0, 0);
        money_scoreboard.AddNew(username, win_scene_info.money_left);
        if (InternetHelper::uploadFile(InternetHelper::upload_php_position, path)) {
            Engine::LOG(Engine::INFO) << "successfully upload the modified money scoreboard";
        } else {
            Engine::LOG(Engine::INFO) << "failed to upload the modified money scoreboard";
        }
    }
    path = "Resource/scoreboard/stage" + std::to_string(win_scene_info.last_stage) + "_moneyLeft_scoreboard.txt";
    Engine::Scoreboard money_scoreboard(path, 0, 0);
    money_scoreboard.AddNew(username, win_scene_info.money_left);



    if (InternetHelper::downloadFile(InternetHelper::server_ip + "/I2P_project/scoreboard/stage" + std::to_string(win_scene_info.last_stage) + "_lifeLeft_scoreboard.txt",
                                     "Resource/scoreboard/online/stage" + std::to_string(win_scene_info.last_stage) + "_lifeLeft_scoreboard.txt")) {
        path = "Resource/scoreboard/online/stage" + std::to_string(win_scene_info.last_stage) + "_lifeLeft_scoreboard.txt";
        Engine::Scoreboard life_scoreboard(path, 0, 0);
        life_scoreboard.AddNew(username, win_scene_info.life_left);
        if (InternetHelper::uploadFile(InternetHelper::upload_php_position, path)) {
            Engine::LOG(Engine::INFO) << "successfully upload the modified life scoreboard";
        } else {
            Engine::LOG(Engine::INFO) << "failed to upload the modified life scoreboard";
        }
    }
    path = "Resource/scoreboard/stage" + std::to_string(win_scene_info.last_stage) + "_lifeLeft_scoreboard.txt";
    Engine::Scoreboard life_scoreboard(path, 0, 0);
    life_scoreboard.AddNew(username, win_scene_info.life_left);


	IScene::Terminate();
	AudioHelper::StopBGM(bgmId);

    delete Lwin;
}
void WinScene::Update(float deltaTime) {
	ticks += deltaTime;
	if (ticks > 4 && ticks < 100 &&
		dynamic_cast<FinalPlayScene*>(Engine::GameEngine::GetInstance().GetScene("final-play"))->MapId == 2) {
		ticks = 100;
		bgmId = AudioHelper::PlayBGM("happy.ogg");
	}
}
void WinScene::ProceedOnClick(int stage) {
    PlotScene* scene = dynamic_cast<PlotScene*>(Engine::GameEngine::GetInstance().GetScene("plot-scene"));
    scene->SetPlotPathTo("Resource/plot/plot" + std::to_string(MapId) + "-e.txt");
    scene->stage = MapId;
    scene->GoToPlayNext = false;
    Engine::GameEngine::GetInstance().ChangeScene("plot-scene");
}
void WinScene::OnKeyDown(int keyCode) {
    switch (keyCode) {
        case ALLEGRO_KEY_ENTER:
            ProceedOnClick(MapId);
            break;

        case ALLEGRO_KEY_LSHIFT:
            if (!Lshift)
            {
                Lshift = true;
                Engine::LOG(Engine::INFO) << ("LShift Down!");
            }
            break;

        case ALLEGRO_KEY_RSHIFT:
            if (!Rshift)
            {
                Rshift = true;
                Engine::LOG(Engine::INFO) << ("RShift Down!");
            }
            break;

        default:
            if (keyCode >= 1 && keyCode <= 26)
            {
                if (!Lshift && !Rshift)
                {
                    Engine::LOG(Engine::INFO) << "Enter:" << (char)(keyCode + 'a' - 1);
                    username += (char)(keyCode + 'a' - 1);
                }
                else
                {
                    Engine::LOG(Engine::INFO) << ("Enter:") << (char)(keyCode + 'A' - 1);
                    username += (char)(keyCode + 'A' - 1);
                }
            }
            else if (keyCode == 75)
            {
                Engine::LOG(Engine::INFO) << ("Enter:space");
                username += ' ';
            }
            else if (keyCode == 63)
            {
                Engine::LOG(Engine::INFO) << ("Enter:backspace");
                if (username != "")
                {
                    username.erase(username.size()-1);
                }
            }
            else if (keyCode >= 37 && keyCode <= 46)
            {
                Engine::LOG(Engine::INFO) << ("Enter:%c") << (char)(keyCode + '0' - 37);
                username += (char)(keyCode + '0' - 37);
            }
            else if (keyCode >= 27 && keyCode <= 36)
            {
                Engine::LOG(Engine::INFO) << ("Enter:%c") << (char)(keyCode + '0' - 27);
                username += (char)(keyCode + '0' - 27);
            }
            else
            {
                Engine::LOG(Engine::INFO) << "Key Not Support: " << keyCode;
            }
            break;
    }
}
void WinScene::OnKeyUp(int keyCode) {
    switch (keyCode) {
        case ALLEGRO_KEY_LSHIFT:
            if (Lshift)
            {
                Lshift = false;
                Engine::LOG(Engine::INFO) << ("LShift Up!");
            }
            break;

        case ALLEGRO_KEY_RSHIFT:
            if (Rshift)
            {
                Rshift = false;
                Engine::LOG(Engine::INFO) << ("RShift Up!");
            }
            break;

        default:
            break;
    }
}