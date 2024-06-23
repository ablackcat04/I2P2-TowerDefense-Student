//
// Created by wuru8 on 2024/5/12.
//

#include "MomoiTurret.hpp"

#include <allegro5/base.h>
#include <cmath>

#include "Engine/AudioHelper.hpp"
#include "Bullet/FireBullet.hpp"
#include "Bullet/MomoiBullet.hpp"
#include "Engine/Group.hpp"
#include "Scene/PlayScene.hpp"

const int MomoiTurret::Price = 250;
MomoiTurret::MomoiTurret(float x, float y) :
        Turret("play/tower-base.png", "play/momoi64x64.png", x, y, 600, Price, 3.5) {
    // Move center downward, since we the turret head is slightly biased upward.
    Anchor.y -= 8.0f / GetBitmapHeight();
    rhythm = 0.5f;
}
void MomoiTurret::CreateBullet() {

    if (getPlayScene()->GetMoney() < 15) {
        return;
    }
    getPlayScene()->EarnMoney(-15);

    Engine::Point diff = Engine::Point(cos(Rotation - ALLEGRO_PI / 2), sin(Rotation - ALLEGRO_PI / 2));
    float rotation = atan2(diff.y, diff.x);
    Engine::Point normalized = diff.Normalize();
    // Change bullet position to the front of the gun barrel.
    getPlayScene()->BulletGroup->AddNewObject(new MomoiBullet(Position + normalized * 36, diff, rotation, this));
    AudioHelper::PlayAudio("gun.wav");
}
