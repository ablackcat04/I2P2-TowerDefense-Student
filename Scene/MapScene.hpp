#ifndef MAPSCENE_HPP
#define MAPSCENE_HPP

#include <memory>
#include <allegro5/allegro_audio.h>
#include "Engine/IScene.hpp"

class MapScene final : public Engine::IScene {
private:
    int maximum_opened_stage = 1;
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;
public:
    explicit MapScene() = default;
    void Initialize() override;
    void Terminate() override;
    void StageOnClick(int stage);
    void BackOnClick();
    void UnlockStage(int map_id);

    void OnKeyDown(int keyCode) override;
};

#endif // MAPSCENE_HPP
