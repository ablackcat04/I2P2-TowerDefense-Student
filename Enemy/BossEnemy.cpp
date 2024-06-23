//
// Created by wuru8 on 2024/6/23.
//

#include "BossEnemy.hpp"
#include "allegro5/allegro_primitives.h"

BossEnemy::BossEnemy(int x, int y) : Enemy("play/boss.png", x, y, 25, 35, 10000, 3500) {

}


void BossEnemy::Draw() const {
    Enemy::Draw();
    al_draw_filled_rounded_rectangle(398, 35, 1002, 45, 0.25, 0.25, al_map_rgb(125, 125, 125));
    al_draw_filled_rounded_rectangle(400, 35, 400+600*(hp/10000), 45, 0.2, 0.2, al_map_rgb(255, 25, 25));
}

void BossEnemy::Update(float deltaTime) {
    Enemy::Update(deltaTime);
}
