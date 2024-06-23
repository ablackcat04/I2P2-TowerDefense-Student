#include <allegro5/base.h>
#include <random>

#include "UI/Animation/DirtyEffect.hpp"
#include "Enemy/Enemy.hpp"
#include "MomoiBullet.hpp"

class Turret;

MomoiBullet::MomoiBullet(Engine::Point position, Engine::Point forwardDirection, float rotation, Turret* parent) :
        Bullet("play/bullet-momoi.png", 1500, 100, position, forwardDirection, rotation - ALLEGRO_PI / 2, parent) {
    CollisionRadius = 8;

}
void MomoiBullet::OnExplode(Enemy* enemy) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(2, 10);
    getPlayScene()->GroundEffectGroup->AddNewObject(new DirtyEffect("play/dirty-2.png", dist(rng), enemy->Position.x, enemy->Position.y));
}
