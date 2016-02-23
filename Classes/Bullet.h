#pragma once

#include "cocos2d.h"

USING_NS_CC;

class AttackableSprite;

class Bullet : public Sprite {
public:

	//virtual bool isCollided(AttackableSprite* sprite) const = 0;

	virtual void start() { runAction(_moveBy); schedule([this](float) { _isDone = true; }, _moveBy->getDuration(), "set _isDone"); }

	virtual bool isDone() { return _isDone; }

	virtual void explode() = 0;

	Bullet(float duration, const Vec2& deltaVec, AttackableSprite* source);

	virtual void initPhysicsBody() = 0;

	~Bullet() override;

protected:
	bool _isDone;
	AttackableSprite* _source;
	MoveBy* _moveBy;
};