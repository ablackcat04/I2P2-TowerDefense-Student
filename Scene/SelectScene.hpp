#ifndef STAGESELECTSCENE_HPP
#define STAGESELECTSCENE_HPP

#include <memory>
#include <allegro5/allegro_audio.h>
#include "Engine/IScene.hpp"

class SelectScene final : public Engine::IScene {
private:
	std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;
public:
	explicit SelectScene() = default;
	void Initialize() override;
	void Terminate() override;
    void StoryModeOnClick();
    void ScoreboardOnClick();
	void BackOnClick();
};

#endif // STAGESELECTSCENE_HPP
