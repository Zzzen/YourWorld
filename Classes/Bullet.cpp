#include "Bullet.h"

#include "Mob.h"

Bullet::Bullet(const float duration, const Vec2& deltaVec, AttackableSprite* source):
	_source(source),
	_moveBy(MoveBy::create(duration, deltaVec)),
	_isDone(false)
{
	scheduleOnce([this](float dt) {
		if (getParent()) {
			removeFromParent();
		}
		else {
			log("%s", "wtf");
		}
	}, duration, "remove for time out");

	auto targetDied = EventListenerCustom::create(MobDieEventData::getName(), [this](EventCustom * event) {
		auto mob = static_cast<MobDieEventData*>(event->getUserData())->getWho();
		assert(mob);
		if (mob == _source) {
			_source = nullptr;
		}
	});

	_eventDispatcher->addEventListenerWithSceneGraphPriority(targetDied, this);
}

Bullet::~Bullet()
{
	log("~Bullet ___ %p", this);
}
