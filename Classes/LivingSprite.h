#pragma once
#include "SerializableSprite.h"

class LivingSprite: public SerializableSprite
{
public:
	~LivingSprite() override {}

	//+HP, age
	unordered_map<string, string> toJson() override;

	CC_PROPERTY(int, _HP, HP);
	CC_SYNTHESIZE(int, _age, Age);

protected:
	virtual void die() = 0;

	LivingSprite() {}
};
