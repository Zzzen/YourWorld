#include "Bullet.h"

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
}

Bullet::~Bullet()
{
	log("~Bullet ___ %p", this);
}
