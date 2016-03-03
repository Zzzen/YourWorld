#pragma once

#include "cocos2d.h"

USING_NS_CC;

class AttackableSprite;


class DamageEventData : public Ref {
public:
	enum Type {
		PHYSICAL = 0,
		MAGICAL = 1
	};

	static const std::string getEventName() { return "damage_event"; }

	virtual float getDamage() { return _damage; }
	virtual Rect getRange() { return _range; }
	virtual AttackableSprite* getSource() { return _source; }

	bool affects(Point p) { return _range.containsPoint(p); }
	bool affects(Rect r) { return _range.intersectsRect(r); }

	//@param source can be nullptr
	static DamageEventData* create(Point pos, Rect range, float damage, AttackableSprite* source);

	virtual ~DamageEventData();

protected:
	//class Effect {
	//	enum Type {
	//		CRITICAL_STRIKE = 0,
	//		BASH = 1,
	//		LIFE_STEAL = 2
	//	};
	//};

	const Point _pos;
	const Rect _range;

	const float _damage;

	AttackableSprite* const _source;

	DamageEventData(Point pos, Rect range, float damage, AttackableSprite* source);
};