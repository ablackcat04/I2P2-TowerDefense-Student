//
// Created by wuru8 on 2024/5/11.
//

#ifndef INC_2024_I2P2_TOWERDEFENSE_SCOREBOARDSCENE_HPP
#define INC_2024_I2P2_TOWERDEFENSE_SCOREBOARDSCENE_HPP
#include <allegro5/allegro_audio.h>
#include <memory>
#include "Engine/IScene.hpp"

class ScoreboardScene final : public Engine::IScene {
private:
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;
    Engine::Label *Lscb = nullptr;
    std::string title_name = "Scoreboard - stage1";

    bool online_avaliable[2][2];

public:
    explicit ScoreboardScene() = default;
    void Initialize() override;
    void Terminate() override;
    static void PrevOnClick();
    static void NextOnClick();
    static void BackOnClick(int stage);
    void Update(float deltaTime) override;
};




#endif //INC_2024_I2P2_TOWERDEFENSE_SCOREBOARDSCENE_HPP
