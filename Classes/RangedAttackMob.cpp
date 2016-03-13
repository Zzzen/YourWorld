#include "RangedAttackMob.h"

static const int MOVE_ACTION_TAG = 334;
static const float DEFAULT_ATTACK_RANGE = 400.f;


RangedAttackMob::RangedAttackMob():
	_attackRange(DEFAULT_ATTACK_RANGE)
{
}


void RangedAttackMob::updateCustom(float dt)
{
	if (getCurrentState() == ATTACK || !_attackTarget) {
		return;
	}

	auto gradient = _attackTarget->getPosition() - getPosition();

	stopActionByTag(MOVE_ACTION_TAG);

	if (gradient.length() > getAlarmDistance()) {
		return;
	}

	if (gradient.length() < _attackRange) {
		startAttacking();
		return;
	}

	auto moveAction = MoveBy::create(0.3f, gradient.getNormalized() * getMoveSpeed());
	moveAction->setTag(MOVE_ACTION_TAG);
	runAction(moveAction);
}

