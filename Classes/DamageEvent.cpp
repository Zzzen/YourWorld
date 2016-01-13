#include "DamageEvent.h"

#include "AttackableSprite.h"

DamageEvent * DamageEvent::create(Point pos, Rect range, float damage, AttackableSprite * source)
{
	auto d = new DamageEvent(pos, range, damage, source);
	d->autorelease();
	return d;
}

DamageEvent::~DamageEvent()
{
	if (_source) {
		_source->release();
	}
}

DamageEvent::DamageEvent(Point pos, Rect range, float damage, AttackableSprite* source) :
	_pos(pos),
	_range(range),
	_damage(damage),
	_source(source)
{
	if (_source) {
		_source->retain();
	}
}

