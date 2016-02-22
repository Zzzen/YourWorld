#pragma once

#include "Bullet.h"

USING_NS_CC;


class Jian : public Bullet {
public:
	//@param direction: in radian
	//@param duration: negative duration flies forover
	static Jian* create(float direction, float velocity, float duration, float damage, AttackableSprite* _source);

	bool isCollided(AttackableSprite* sprite) const override;
	Jian(float duration, const Vec2& deltaVec, AttackableSprite* source);

	~Jian() override;

protected:
	bool init() override;

	void explode() override;
	float _damage;
};