#ifndef MAPSCENE_HPP
#define MAPSCENE_HPP
#include <allegro5/allegro_audio.h>
#include <memory>
#include "Engine/IScene.hpp"
class MapScene final : public Engine::IScene {
private:
    int cnt = 1;
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;
public:
    explicit MapScene() = default;
    void Initialize() override;
    void Terminate() override;
    void PlayOnClick(int stage);
    void BackOnClick(int stage);
    void IncCount();
    int GetCount();
};

#endif // MAPSCENE_HPP
