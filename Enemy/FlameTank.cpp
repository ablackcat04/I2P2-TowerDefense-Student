//
// Created by wuru8 on 2024/5/12.
//
#include <allegro5/base.h>
#include <random>

#include "Scene/PlayScene.hpp"
#include "FlameTank.hpp"

FlameTank::FlameTank(int x, int y) : Enemy("play/enemy-6.png", x, y, 25, 25, 300, 50),
                                     head("play/enemy-2.png", x, y), targetRotation(0), count(0.0f) {
}
void FlameTank::Draw() const {
    Enemy::Draw();
}
void FlameTank::Update(float deltaTime) {
    Enemy::Update(deltaTime);
    //head.Position = Position;
    // Choose arbitrary one.
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_real_distribution<> dist(0.0f, 4.0f);
    float rnd = dist(rng);
    if (rnd < deltaTime) {
        // Head arbitrary rotation.
        std::uniform_real_distribution<> distRadian(-ALLEGRO_PI, ALLEGRO_PI);
        targetRotation = distRadian(rng);
    }
    head.Rotation = (head.Rotation + deltaTime * targetRotation) / (1 + deltaTime);

    count += deltaTime;

    if (count >= 2.0f) {
        count = 0;
        getPlayScene()->EarnMoney(-10);
        money += 6;
    }
}
