//
// Created by wuru8 on 2024/6/23.
//

#ifndef INC_2024_I2P2_TOWERDEFENSE_BOSSENEMY_HPP
#define INC_2024_I2P2_TOWERDEFENSE_BOSSENEMY_HPP

#include "Enemy.hpp"
#include "Engine/Sprite.hpp"

class BossEnemy : public Enemy {
private:
    bool shield;

    bool triggeredBy1;
public:
    BossEnemy(int x, int y);
    void Draw() const override;
    void Update(float deltaTime) override;

    void TriggerByRhythm(float R) override;
};

#endif //INC_2024_I2P2_TOWERDEFENSE_BOSSENEMY_HPP
