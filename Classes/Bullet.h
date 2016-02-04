#pragma once

#include "cocos2d.h"

USING_NS_CC;

class AttackableSprite;

class Bullet : public Sprite {
public:

	virtual bool isCollided(AttackableSprite* sprite) const = 0;

	virtual void start() { runAction(_moveBy); }

	Bullet(float duration, const Vec2& deltaVec, AttackableSprite* source);
protected:
	virtual void explode() = 0;

	AttackableSprite* _source;
	MoveBy* _moveBy;
};