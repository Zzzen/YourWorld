#include "StatusEffect.h"
#include "AttackableSprite.h"

void StatusEffect::setTarget(AttackableSprite * target)
{
	_target = target;
	begin();

	schedule(CC_SCHEDULE_SELECTOR(StatusEffect::updateCustomCaller), 1.0f, kRepeatForever, 1.0f);

	if (getScheduler()->isTargetPaused(target)) pause();
}

void StatusEffect::refresh()
{
	_duration = 0;
}

void StatusEffect::updateCustom(float dt)
{
	_duration += dt;

	if (_duration > getMaxDuration()) {
		end();
		_target->removeStatusEffect(this);
	}
}

StatusEffect::StatusEffect():
	_duration(0.0f),
	_target(nullptr)
{
}
