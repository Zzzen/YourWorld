#include "Bullet.h"

Bullet::Bullet(const float duration, const Vec2& deltaVec, AttackableSprite* source):
	_source(source),
	_moveBy(MoveBy::create(duration, deltaVec))
{
	scheduleOnce([this](float dt) { if (getParent()) removeFromParent(); }, duration, "removeFromParent");
}
