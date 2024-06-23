//
// Created by wuru8 on 2024/6/23.
//

#include "BossEnemy.hpp"
#include "allegro5/allegro_primitives.h"
#include "Engine/Sprite.hpp"

BossEnemy::BossEnemy(int x, int y) : Enemy("play/boss.png", x, y, 25, 35, 10000, 3500) {
    shield = true;
    triggeredBy1 = false;
    rhythm = 2.f;
}


void BossEnemy::Draw() const {
    Enemy::Draw();
    al_draw_filled_rounded_rectangle(398, 35, 1002, 45, 0.25, 0.25, al_map_rgb(125, 125, 125));
    al_draw_filled_rounded_rectangle(400, 35, 400+600*(hp/10000), 45, 0.2, 0.2, al_map_rgb(255, 25, 25));

    if (shield) {
        al_draw_filled_rounded_rectangle(Position.x + 12, Position.y + 5, Position.x + 28, Position.y + 64, 0.1f, 0.1f, al_map_rgb(200, 150, 170));
    }
}

void BossEnemy::Update(float deltaTime) {
    Enemy::Update(deltaTime);
    if (shield) {
        damaged_ratio = 0.5;
    } else {
        damaged_ratio = 2.0;
    }
    triggeredBy1 = false;
}

void BossEnemy::TriggerByRhythm(float R) {
    if (R == 1.f) {
        triggeredBy1 = true;
    } else if (R == rhythm) {
        shield = triggeredBy1;
    }
}