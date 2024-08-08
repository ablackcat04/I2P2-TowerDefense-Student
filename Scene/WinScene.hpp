#ifndef WINSCENE_HPP
#define WINSCENE_HPP

#include <allegro5/allegro_audio.h>
#include "Engine/IScene.hpp"


struct WinSceneInfo {
    int last_stage;
    int money_left;
    int life_left;
};

class WinScene final : public Engine::IScene {
private:
	float ticks;
	ALLEGRO_SAMPLE_ID bgmId;
protected:
    std::string username = "";
    bool Rshift = false;
    bool Lshift = false;
public:
	explicit WinScene() = default;
	void Initialize() override;
	void Terminate() override;
	void Update(float deltaTime) override;
	void ProceedOnClick(int stage);
    void OnKeyDown(int keyCode) final override;
    void OnKeyUp(int keyCode) final override;

    void SetLastStage(int stage);
    void SetLastGameInfo(int money, int life);

    int MapId;
};

#endif // WINSCENE_HPP
