#include "DamageEventData.h"

#include "AttackableSprite.h"

DamageEventData * DamageEventData::create(Point pos, Rect range, float damage, AttackableSprite * source)
{
	auto d = new DamageEventData(pos, range, damage, source);
	d->autorelease();
	return d;
}

DamageEventData::~DamageEventData()
{
	if (_source) {
		_source->release();
	}
}

DamageEventData::DamageEventData(Point pos, Rect range, float damage, AttackableSprite* source) :
	_pos(pos),
	_range(range),
	_damage(damage),
	_source(source)
{
	if (_source) {
		_source->retain();
	}
}

