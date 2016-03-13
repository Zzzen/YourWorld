#pragma once

#include "Mob.h"

class RangedAttackMob : public Mob {
public:
	RangedAttackMob();
	
protected:
	float _attackRange;

	void updateCustom(float dt) override;
};