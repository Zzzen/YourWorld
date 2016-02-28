#pragma once

#include "ThrowableItem.h"

USING_NS_CC;

class Crop;
class Seed : public ThrowableItem {
public:
	void beThrowed(AttackableSprite* owner, const Vec2& dst) override;

protected:
	virtual Crop* createCrop() = 0;
};