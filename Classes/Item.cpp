#include "Item.h"

#include "cocos2d.h"
USING_NS_CC;

static const float ROTATE_DURATION = 2.0f;
static const float BLINK_DURATION = 2.0f;
static const int BLINK_TIMES = 8;
static const int ENTER_ACTIONS = 233;

void Item::onEnter() {
	SerializableSprite::onEnter();

	auto rotate = RepeatForever::create(RotateBy::create(ROTATE_DURATION, Vec3(0, 360, 0)));
	//auto blink = RepeatForever::create(Blink::create(BLINK_DURATION, BLINK_TIMES));
	
	rotate->setTag(ENTER_ACTIONS);
	//blink->setTag(ENTER_ACTIONS);
	runAction(rotate);
	//runAction(blink);
}

void Item::onExit() {
	SerializableSprite::onExit();

	stopAllActionsByTag(ENTER_ACTIONS);
}
