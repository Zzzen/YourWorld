#include "Human.h"
#include "DamageEventData.h"


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
