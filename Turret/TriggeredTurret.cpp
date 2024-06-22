//
// Created by wuru8 on 2024/6/21.
//

#include "TriggeredTurret.hpp"
#include <allegro5/base.h>
#include <list>
#include <string>
#include "Turret.hpp"
#include <cmath>
#include "Enemy/Enemy.hpp"
#include "Engine/LOG.hpp"

TriggeredTurret::TriggeredTurret(std::string imgBase, std::string imgTurret, float x, float y, float radius, int price,
                                 float coolDown) : Turret(imgBase, imgTurret, x, y, radius, price, coolDown) {
    triggered = 0;
}

void TriggeredTurret::Update(float deltaTime) {
    Sprite::Update(deltaTime);
    FinalPlayScene* scene = getPlayScene();
    imgBase.Position = Position;
    imgBase.Tint = Tint;
    if (!Enabled)
        return;
    if (Target) {
        Engine::Point diff = Target->Position - Position;
        if (diff.Magnitude() > CollisionRadius) {
            Target->lockedTurrets.erase(lockedTurretIterator);
            Target = nullptr;
            lockedTurretIterator = std::list<Turret*>::iterator();
        }
    }
    if (!Target) {
        // Lock first seen target.
        // Can be improved by Spatial Hash, Quad Tree, ...
        // However, simply loop through all enemies is enough for this program.
        for (auto& it : scene->EnemyGroup->GetObjects()) {
            Engine::Point diff = it->Position - Position;
            if (diff.Magnitude() <= CollisionRadius) {
                Target = dynamic_cast<Enemy*>(it);
                Target->lockedTurrets.push_back(this);
                lockedTurretIterator = std::prev(Target->lockedTurrets.end());
                break;
            }
        }
    }
    if (Target) {
        Engine::Point originRotation = Engine::Point(cos(Rotation - ALLEGRO_PI / 2), sin(Rotation - ALLEGRO_PI / 2));
        Engine::Point targetRotation = (Target->Position - Position).Normalize();
        float maxRotateRadian = rotateRadian * deltaTime;
        float cosTheta = originRotation.Dot(targetRotation);
        // Might have floating-point precision error.
        if (cosTheta > 1) cosTheta = 1;
        else if (cosTheta < -1) cosTheta = -1;
        float radian = acos(cosTheta);
        Engine::Point rotation;
        if (abs(radian) <= maxRotateRadian)
            rotation = targetRotation;
        else
            rotation = ((abs(radian) - maxRotateRadian) * originRotation + maxRotateRadian * targetRotation) / radian;
        // Add 90 degrees (PI/2 radian), since we assume the image is oriented upward.
        Rotation = atan2(rotation.y, rotation.x) + ALLEGRO_PI / 2;
        // Shoot reload.
        if (triggered >= notes_to_trigger) {
            CreateBullet();
        }
        triggered = 0;
    }
}

void TriggeredTurret::TriggerByHit() {
    triggered += 1;
}

void TriggeredTurret::TriggerByRhythm(float R) {
    return;
}