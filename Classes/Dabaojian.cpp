#include "Dabaojian.h"
#include "cocos2d.h"

const float Dabaojian::STRENGTH = 100.0f;

Dabaojian * Dabaojian::create()
{
	auto d = new Dabaojian;
	if (d && d->initWithFile("dabaojian.png")) {
		d->_equipmentType = AttackableSprite::EquipmentType::WEAPON;
		d->autorelease();
		return d;
	}
	else {
		CCASSERT(false, "dabaojian.png is missing");
		CC_SAFE_DELETE(d);
		return nullptr;
	}
}

void Dabaojian::attach(AttackableSprite * owner)
{
	owner->addStrength(STRENGTH);
}

void Dabaojian::detach(AttackableSprite * owner)
{
	owner->addStrength(-STRENGTH);
}
