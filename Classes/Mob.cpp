#include "Mob.h"
#include "You.h"
#include "Utils.h"

static const int MOVE_ACTION_TAG = 333;

void Mob::updateCustom(float dt) {
	if (getCurrentState() == ATTACK) {
		return;
	}

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

	auto moveAction = MoveBy::create(0.3f, gradient.getNormalized() * getMoveSpeed());
	moveAction->setTag(MOVE_ACTION_TAG);
	runAction(moveAction);
}

float Mob::getAlarmDistance() const {
	return 400.0f;
}

void Mob::die() {
	auto event = MobDieEvent::createWithWho(this);
	Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(MobDieEvent::getName(),
																	   event);
}