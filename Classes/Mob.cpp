#include "Mob.h"
#include "You.h"
#include "Utils.h"

static const int MOVE_ACTION_TAG = 333;

void Mob::updateCustom(float dt) {
	if (getCurrentState() == ATTACK || !_attackTarget) {
		return;
	}

	auto gradient = _attackTarget->getPosition() - getPosition();

	stopActionByTag(MOVE_ACTION_TAG);

	if (gradient.length() > getAlarmDistance()) {
		return;
	}

	if (gradient.length() < 30) {
		startAttacking();
		return;
	}

	auto moveAction = MoveBy::create(0.3f, gradient.getNormalized() * getOriginalMoveSpeed());
	moveAction->setTag(MOVE_ACTION_TAG);
	runAction(moveAction);
}

float Mob::getAlarmDistance() const {
	return 400.0f;
}

Mob::Mob():
	_attackTarget(You::getInstance())
{
	auto targetDied = EventListenerCustom::create(MobDieEventData::getName(), [this](EventCustom * event) {
		auto mob = static_cast<MobDieEventData*>(event->getUserData())->getWho();
		assert(mob);
		if ( mob == _attackTarget ) {
			_attackTarget = nullptr;
		}
	});

	_eventDispatcher->addEventListenerWithSceneGraphPriority(targetDied, this);
}

void Mob::die() {
	auto event = MobDieEventData::createWithWho(this);
	Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(MobDieEventData::getName(),
																	   event);
}