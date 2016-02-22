#include "Bullet.h"
#include "BulletManager.h"

Bullet::Bullet(const float duration, const Vec2& deltaVec, AttackableSprite* source):
	_source(source),
	_moveBy(MoveBy::create(duration, deltaVec))
{
}

void Bullet::onEnter()
{
	BulletManager::getInstance()->addBullet(this);
	
	_moveBy->retain();

	Sprite::onEnter();
}

void Bullet::onExit()
{
	_moveBy->release();

	Sprite::onExit();
}
