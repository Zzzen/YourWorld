#pragma once

#include "cocos2d.h"
#include "SpriteEvent.h"
#include "AttackableSprite.h"

USING_NS_CC;

class MiningEventData : public SpriteEvent<AttackableSprite> {
public:
	MiningEventData* create(AttackableSprite* source, const Rect& affectedArea);

	Rect getAffectedArea();

	MiningEventData(const Rect& affectedArea);

	static string getName() { return "MiningEventData"; }

protected:
	Rect _affectedArea;
};