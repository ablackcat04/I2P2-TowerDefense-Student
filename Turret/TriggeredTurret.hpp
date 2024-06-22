//
// Created by wuru8 on 2024/6/21.
//

#ifndef INC_2024_I2P2_TOWERDEFENSE_TRIGGEREDTURRET_HPP
#define INC_2024_I2P2_TOWERDEFENSE_TRIGGEREDTURRET_HPP

#include <allegro5/base.h>
#include <list>
#include <string>

#include "Engine/Sprite.hpp"
#include "Scene/FinalPlayScene.hpp"
#include "Turret.hpp"

class Enemy;
class PlayScene;

class TriggeredTurret: public Turret {
protected:
    int notes_to_trigger = 1;

public:
    TriggeredTurret(std::string imgBase, std::string imgTurret, float x, float y, float radius, int price, float coolDown);
    void Update(float deltaTime) override;

    void TriggerByHit() override;
    void TriggerByRhythm(float R) override;
};

#endif //INC_2024_I2P2_TOWERDEFENSE_TRIGGEREDTURRET_HPP
