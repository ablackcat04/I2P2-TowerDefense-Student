// This is the entry point of your game.
// You can register your scenes here, and start the game.
#include "Engine/GameEngine.hpp"
#include "Engine/LOG.hpp"
#include "Scene/LoseScene.hpp"
#include "Scene/PlayScene.hpp"
#include "Scene/StageSelectScene.hpp"
#include "Scene/WinScene.hpp"
#include "Scene/StartScene.hpp"
#include "Scene/SettingsScene.hpp"
#include "Scene/ScoreboardScene.hpp"
#include "Scene/PlotScene.hpp"
#include "Scene/MapScene.hpp"

int main() {
	Engine::LOG::SetConfig(true);
	Engine::GameEngine& game = Engine::GameEngine::GetInstance();

    game.AddNewScene("start", new StartScene());
    game.AddNewScene("stage-select", new StageSelectScene());
	game.AddNewScene("settings", new SettingsScene());
	game.AddNewScene("play-scene", new PlayScene());
	game.AddNewScene("lose", new LoseScene());
	game.AddNewScene("win", new WinScene());
    game.AddNewScene("lose-scene", new LoseScene());
    game.AddNewScene("scoreboard-scene", new ScoreboardScene());
    game.AddNewScene("plot-scene", new PlotScene());
	game.AddNewScene("map-scene", new MapScene());

    auto* scene = reinterpret_cast<PlotScene*>(Engine::GameEngine::GetInstance().GetScene("plot-scene"));
    scene->SetPlotPathTo("Resource/plot/prologue.txt");
    scene->SetNextSceneTo("start");

	game.Start("plot-scene", 60, 1600, 832);
	return 0;
}
