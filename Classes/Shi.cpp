#include "Shi.h"
#include "AttackableSprite.h"

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

void Shi::beConsumed(AttackableSprite* user)
{
	user->addHP(user->getMaxHP()*0.05f);
}
