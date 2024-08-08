// This is the entry point of your game.
// You can register your scenes here, and start the game.
#include "Engine/GameEngine.hpp"
#include "Engine/LOG.hpp"
#include "Scene/LoseScene.hpp"
#include "Scene/PlayScene.hpp"
#include "Scene/SelectScene.hpp"
#include "Scene/WinScene.hpp"
#include "Scene/StartScene.hpp"
#include "Scene/SettingsScene.hpp"
#include "Scene/ScoreboardScene.hpp"
#include "Scene/PlotScene.hpp"
#include "Scene/MapScene.hpp"
#include "Utility/UsefulConstants.hpp"

int main() {
	Engine::LOG::SetConfig(true);
	Engine::GameEngine& game = Engine::GameEngine::GetInstance();

    game.AddNewScene(SceneNames::start, new StartScene());
    game.AddNewScene(SceneNames::select, new SelectScene());
	game.AddNewScene(SceneNames::settings, new SettingsScene());
	game.AddNewScene(SceneNames::play, new PlayScene());
	game.AddNewScene(SceneNames::win, new WinScene());
    game.AddNewScene(SceneNames::lose, new LoseScene());
    game.AddNewScene(SceneNames::scoreboard, new ScoreboardScene());
    game.AddNewScene(SceneNames::plot, new PlotScene());
	game.AddNewScene(SceneNames::map, new MapScene());

    auto* scene = reinterpret_cast<PlotScene*>(Engine::GameEngine::GetInstance().GetScene(SceneNames::plot));
    scene->SetPlotPathTo("Resource/plot/prologue.txt");
    scene->SetNextSceneTo(SceneNames::start);

	game.Start(SceneNames::plot, 60, 1600, 832);
	return 0;
}
