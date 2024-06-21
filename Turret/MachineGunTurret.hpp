#ifndef MACHINEGUNTURRET_HPP
#define MACHINEGUNTURRET_HPP
#include "Turret.hpp"
#include "TriggeredTurret.hpp"

class MachineGunTurret: public TriggeredTurret {
public:
	static const int Price;
    MachineGunTurret(float x, float y);
	void CreateBullet() override;
};
#endif // MACHINEGUNTURRET_HPP
