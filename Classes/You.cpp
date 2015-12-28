#include "You.h"

const std::string YourMoveEvent::EVENT_YOURMOVE = "event_of_yourmove";

//**********************************************

bool You::init(){
	if (!Sprite::init()){
		return false;
	}

	return true;
}

void You::setPosition(const Vec2& position){
	auto oldPosition = getPosition();

	if (oldPosition == position){
		return;
	}

	auto moveEvent = YourMoveEvent::create();
	moveEvent->offset = position - oldPosition;
	Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(
		YourMoveEvent::EVENT_YOURMOVE, moveEvent);

	Node::setPosition(position);
}