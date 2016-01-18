#ifndef __YOU_H__
#define __YOU_H__

#include "cocos2d.h"

#include "AttackableSprite.h"
#include "SpriteEvent.h"

USING_NS_CC;



class You : public AttackableSprite {
public:
	static You* getInstance();

	void setPosition(const Vec2& position) override;

protected:
	static You* create();

	string getSkeletalFileName() const override  { return "res/yourSkeleton.csb"; }
	const string getClassName() const override { return "You"; }


	void updateCustom(float dt) override;
	float getUpdateInterval() override { return 1.0f; }

	float getStrength()const override { return 10.0f; }
	float getMoveSpeed() const override { return 15.0f; }
	int getMaxHP() const override { return 10000; }
	void die() override {}
	
	void attack() override;
};



class YourMoveEvent : public SpriteEvent<You> {
public:
	static YourMoveEvent* createWithWho(You* const sprite) {
		auto event = new YourMoveEvent();
		event->initWithWho(sprite);

		event->autorelease();
		return event;
	}

	static const std::string getName() {
		return "your_move_event";
	}

	Point offset;
};

#endif