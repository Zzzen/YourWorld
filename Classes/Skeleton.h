#pragma once

#include "RangedAttackMob.h"

USING_NS_CC;

class Skeleton : public RangedAttackMob {
public:
	CREATE_FUNC(Skeleton);
	CREATE_WITH_JSON_FUNC(Skeleton);

	void attack() override;

	Skeleton();

	string getClassName()const override { return "Skeleton"; }

protected:
	bool init() override;

	int getOriginalMaxHP() const override { return 50; }
	float getOriginalStrength() const override { return 15.f; }
	AnimationConfig getAnimationConfig() const override { return AnimationConfig::skeleton; }

	void updateCustom(float dt) override;

	Node* _weaponNode;
};