#pragma once

#include "cocos2d.h"
#include "SpriteEventData.h"
#include "AttackableSprite.h"

USING_NS_CC;

class MiningEventData : public SpriteEventData<AttackableSprite> {
public:
	enum ToolType
	{
		EMPTY = 0
	};

	static MiningEventData* create(AttackableSprite* source, const Rect& affectedArea);

	Rect getAffectedArea();

	int getDamage() const { return _damage; }
	void setDamage(int damage) { _damage = damage; }

	MiningEventData(const Rect& affectedArea);

	static string getEventName() { return "MiningEventData"; }

	void setToolType(ToolType type) { _toolType = type; }
	ToolType getToolType() const { return _toolType; }

protected:
	Rect _affectedArea;
	ToolType _toolType;
	int _damage;
};