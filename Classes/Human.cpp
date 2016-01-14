#include "Human.h"
#include "DamageEvent.h"


Human * Human::create()
{
	auto h = new Human();
	if ( h && h->init() ) {
		h->autorelease();
		return h;
	}
	else
	{
		CC_SAFE_DELETE(h);
		return nullptr;
	}
}

void Human::attack() {
	auto damage = DamageEvent::create(getPosition(), getBoundingBox(), 100, this);
	Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(DamageEvent::getEventName(),
		damage);
}