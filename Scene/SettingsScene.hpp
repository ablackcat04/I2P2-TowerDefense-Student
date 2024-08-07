#ifndef SettingsScene_HPP
#define SettingsScene_HPP

#include <memory>
#include <allegro5/allegro_audio.h>
#include "Engine/IScene.hpp"

class SettingsScene final : public Engine::IScene {
private:
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;
public:
    explicit SettingsScene() = default;

    void Initialize() override;

    void Terminate() override;

    void BackOnClick();

    void BGMSlideOnValueChanged(float value);

    void SFXSlideOnValueChanged(float value);
};

#endif // SettingsScene_HPP
