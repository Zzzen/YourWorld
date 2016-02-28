#pragma once

#include "LivingSprite.h"

USING_NS_CC;

class Crop : public LivingSprite {
public:
	virtual bool isPluckable() const = 0;
	//set _lastPluckedTime
	virtual void bePlucked();

	//in millionsecond
	virtual void setLastPluckedTime(long long time);

	//enabled by default
	void setTouchable(bool enabled);

	bool init() override;
	
	bool initWithJson(const Document& json) override;

	Document toJson()const override;

	void updateCustom(float dt) override;

	Crop();

	void onEnter() override;

	//Document toJson() const override;

protected:
	EventListenerTouchOneByOne* _touchListener;

	float getUpdateInterval() const override { return 1.f; }

	PhysicsBodyConfig getPhysicsBodyConfig() const override { return PhysicsBodyConfig::CROP; }

	int getOriginalMaxHP() const { return 100; }
	//in millionsecond
	long long _lastPluckedTime;
};