#pragma once

#include "Mob.h"

class Human: public Mob{
public:
	static Human* create();

protected:
	string getSkeletalFileName() const override { return "res/yourSkeleton.csb"; }
	const string getClassName() const override { return "Human"; }

	float getStrength()const override { return 10.0f; }
	int getMaxHP()const override { return 30; }

	void attack() override;

	~Human() { CCLOG("~Human()"); }
};