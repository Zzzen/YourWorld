#include "Mob.h"
#include "You.h"
#include "Utils.h"

static const int MOVE_ACTION_TAG = 333;

void Mob::updateCustom(float dt) {
	auto you = You::getInstance();
	auto gradient = you->getPosition() - getPosition();

	stopActionByTag(MOVE_ACTION_TAG);

	if (gradient.length() > getAlarmDistance()) {
		return;
	}

	if (gradient.length() < 30) {
		startAttacking();
		return;
	}

	auto moveAction = MoveBy::create(0.3f, gradient.getNormalized() * 10);
	moveAction->setTag(MOVE_ACTION_TAG);
	runAction(moveAction);
}

float Mob::getAlarmDistance() const {
	return 400.0f;
}