// [main.cpp]
// This is the entry point of your game.
// You can register your scenes here, and start the game.
#include "Engine/GameEngine.hpp"
#include "Engine/LOG.hpp"
#include "Scene/LoseScene.hpp"
#include "Scene/PlayScene.hpp"
#include "Scene/StageSelectScene.hpp"
#include "Scene/WinScene.hpp"
#include "Scene/StartScene.h"
#include "Scene/SettingsScene.hpp"
#include "Scene/ScoreboardScene.hpp"
#include "Scene/PlotScene.hpp"
#include "Utility/InternetHelper.hpp"
#include "curl/curl.h"

int main() {
	Engine::LOG::SetConfig(true);
	Engine::GameEngine& game = Engine::GameEngine::GetInstance();

    InternetHelper::downloadFile("http://192.168.56.1/I2P_project/hello_server.txt", "Resource/scoreboard/online/hello_server.txt");

    // ODO: [HACKATHON-1-SCENE] (3/4): Register Scenes here
    game.AddNewScene("start", new StartScene());
    game.AddNewScene("stage-select", new StageSelectScene());
	game.AddNewScene("settings", new SettingsScene());
	game.AddNewScene("play", new PlayScene());
	game.AddNewScene("lose", new LoseScene());
	game.AddNewScene("win", new WinScene());
    game.AddNewScene("lose-scene", new LoseScene());
    game.AddNewScene("scoreboard-scene", new ScoreboardScene());
    game.AddNewScene("plot-scene", new PlotScene());

    // ODO: [HACKATHON-1-SCENE] (4/4): Change the start scene
	game.Start("start", 60, 1600, 832);
	return 0;
}
