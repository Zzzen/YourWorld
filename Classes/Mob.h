#pragma once

#include "AttackableSprite.h"

USING_NS_CC;

class Mob : public AttackableSprite {
protected:
	void updateCustom(float dt) override;
	float getUpdateInterval() override { return 0.1f; }

	virtual float getAlarmDistance() const;

};
