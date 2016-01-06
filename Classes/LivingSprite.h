#pragma once
#include "SerializableSprite.h"

class LivingSprite: public SerializableSprite
{
public:
	~LivingSprite() override {}

	//+HP, age
	Document toJson() const override;

	CC_PROPERTY(int, _HP, HP);
	CC_SYNTHESIZE(int, _age, Age);

protected:
	virtual void die() = 0;

	virtual void updateCustom(float dt);
	virtual float getUpdateInterval() = 0;

	bool initWithJson(const Document& json);

private:
	void callUpdateCustom(float dt) { updateCustom(dt); }
};
