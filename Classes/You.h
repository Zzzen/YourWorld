#ifndef __YOU_H__
#define __YOU_H__

#include "cocos2d.h"

#include "AttackableSprite.h"
#include "SpriteEvent.h"
#include "ui\UIScrollView.h"
#include "ThrowableItem.h"


USING_NS_CC;
using namespace cocos2d::ui;


class You : public AttackableSprite {
public:
	static You* getInstance();

	void setPosition(float x, float y) override;

	void setPosition(const Vec2& pos) override { setPosition(pos.x, pos.y); }

	ScrollView* showInventory();

	bool initWithJson(const Document & json) override;

	Sprite* getItemLabel() const { return _itemLabel; }

	static You* create();
	You();
	~You();

protected:

	AnimationConfig getAnimationConfig() const override { return AnimationConfig::yourSkeleton; }
	PhysicsBodyConfig getPhysicsBodyConfig() const override { return PhysicsBodyConfig::YOU; }
	string getClassName() const override { return "You"; }

	bool init() override;

	void updateCustom(float dt) override;
	float getUpdateInterval()const override { return 1.0f; }

	void die() override {}

	//display which is being prepared to throw
	Sprite* _itemLabel;

	ThrowableItem* _itemToThrow;

private:
	int getOriginalMaxHP() const override { return 10000; }
	float getOriginalStrength()const override { return 10.0f; }
	float getOriginalMoveSpeed() const override { return 15.0f; }
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