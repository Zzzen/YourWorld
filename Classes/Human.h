#pragma once

#include "Mob.h"

class Human: public Mob{
public:
	static Human* create();
	static Human* createWithJson(const Document& json);

protected:
	string getSkeletalFileName() const override { return "res/yourSkeleton.csb"; }
	const string getClassName() const override { return "Human"; }
	
	void attack() override;

private:
	int getOriginalMaxHP()const override { return 30; }
	float getOriginalStrength()const override { return 10.0f; }
};