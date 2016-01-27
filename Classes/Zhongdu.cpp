#include "Zhongdu.h"
#include "AttackableSprite.h"

void Zhongdu::begin()
{
	getTarget()->addStrength(-9);
	getTarget()->addHP(-100);
}

void Zhongdu::end()
{
	getTarget()->addStrength(9);
}

void Zhongdu::updateCustom(float dt)
{
	getTarget()->addHP(-10);

	StatusEffect::updateCustom(dt);
}
