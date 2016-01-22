#pragma once

#include "AttackableSprite.h"
#include "SpriteEvent.h"

USING_NS_CC;

class Mob : public AttackableSprite {
public:
	virtual float getAlarmDistance() const;


protected:
	void updateCustom(float dt) override;
	float getUpdateInterval() override { return 0.1f; }

	void die() override;

private:
	float getOriginalMoveSpeed()const override { return 10.0f; }
};

class MobDieEvent : public SpriteEvent<Mob>
{
public:
	static MobDieEvent* createWithWho(Mob* const sprite) {
		auto event = new MobDieEvent();
		event->initWithWho(sprite);

		event->autorelease();
		return event;
	}

	static const std::string getName() {
		return "mob_die_event";
	}
};
