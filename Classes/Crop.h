#pragma once

#include "LivingSprite.h"

USING_NS_CC;

class Crop : public LivingSprite {
public:
	virtual bool isPluckable() const = 0;
	virtual void bePlucked() = 0;

	void setTouchable(bool enabled);

	bool init() override;

	void updateCustom(float dt) override;

	Crop();

	void onEnter() override;

protected:
	EventListenerTouchOneByOne* _touchListener;

	float getUpdateInterval() const override { return 1.f; }

	int getOriginalMaxHP() const { return 100; }
};