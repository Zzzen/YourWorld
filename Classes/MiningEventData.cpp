#include "MiningEventData.h"

MiningEventData * MiningEventData::create(AttackableSprite * source, const Rect & affectedArea)
{
	auto event = new MiningEventData(affectedArea);
	event->initWithWho(source);
	event->autorelease();
	
	return event;
}

MiningEventData::MiningEventData(const Rect & affectedArea):
	_affectedArea(affectedArea)
{
}
