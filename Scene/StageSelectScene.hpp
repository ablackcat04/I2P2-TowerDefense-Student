#ifndef STAGESELECTSCENE_HPP
#define STAGESELECTSCENE_HPP

#include <memory>
#include <allegro5/allegro_audio.h>
#include "Engine/IScene.hpp"

class StageSelectScene final : public Engine::IScene {
private:
	std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;
public:
	explicit StageSelectScene() = default;
	void Initialize() override;
	void Terminate() override;
    void PlayOnClick(int stage);
    void ScoreboardOnClick();
	void BackOnClick(int stage);
    void TestOnClick();
	void TestMapOnClick();
    void RhythmGameOnClick();
};

#endif // STAGESELECTSCENE_HPP
