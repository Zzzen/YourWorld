#include "You.h"

void You::setPosition(const Vec2& position){
	auto oldPosition = getPosition();

	if (oldPosition == position){
		return;
	}

	float flip = position.x > oldPosition.x ? 1:-1;
	_skeletalNode->setScaleX(flip);

	auto moveEvent = YourMoveEvent::createWithWho(this);
	moveEvent->offset = position - oldPosition;
	Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(
		YourMoveEvent::getName(), moveEvent);

	AttackableSprite::setPosition(position);
}

You* You::create() {
	auto you = new You;
	if (you && you->init()) {
		you->autorelease();
		return you;
	}
	else
	{
		CC_SAFE_DELETE(you);
		return nullptr;
	}
}

void You::updateCustom(float dt) {
}