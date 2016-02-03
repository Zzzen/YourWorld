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

Human * Human::createWithJson(const Document & json)
{
	auto h = Human::create();
	if (!h) return nullptr;

	h->initWithJson(json);

	return h;
}

void Human::attack() {
	auto damage = DamageEvent::create(getPosition(), getBoundingBox(), getStrength(), this);
	Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(DamageEvent::getEventName(),
		damage);
}