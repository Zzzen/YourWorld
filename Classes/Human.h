#pragma once

#include "Mob.h"

class Human: public Mob{
public:
	static Human* create();

	void die() override { removeFromParent(); }

protected:
	string getSkeletalFileName() const override { return "res/yourSkeleton.csb"; }
	const string getClassName() const override { return "Human"; }

	float getStrength()const override { return 10.0f; }

	void attack() override;

	Human() { setHP(30.0f); }
	~Human() { CCLOG("~Human()"); }
};