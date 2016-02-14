#include "Shi.h"
#include "AttackableSprite.h"
#include "Zhongdu.h"

Shi * Shi::create()
{
	auto shi = new Shi();
	if (shi&&shi->initWithFile("shi.png")) {
		shi->autorelease();
		return shi;
	}
	else {
		CC_SAFE_DELETE(shi);
		return nullptr;
	}
}

Shi * Shi::createWithJson(const Document & doc)
{
	return create();
}

void Shi::beConsumed(AttackableSprite* user)
{
	auto du = Zhongdu::create();
	user->addStatusEffect(du);
	user->addHP(user->getMaxHP()*0.05f);
}
