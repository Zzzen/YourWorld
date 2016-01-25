#include "You.h"
#include "DamageEvent.h"
#include "Utils.h"
#include "ui\UIButton.h"
#include "ui\UIWidget.h"
#include "Equipment.h"

void You::setPosition(const Vec2& position){
	auto oldPosition = getPosition();

	if (oldPosition == position){
		return;
	}

	auto moveEvent = YourMoveEvent::createWithWho(this);
	moveEvent->offset = position - oldPosition;
	Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(
		YourMoveEvent::getName(), moveEvent);

	AttackableSprite::setPosition(position);
}

ScrollView * You::showInventory()
{
	using namespace cocos2d::ui;
	auto view = ScrollView::create();
	view->setContentSize(getVisibleSize()/2.0f);
	
	const static int ICON_SIZE = 32;
	float innerHeight = ICON_SIZE*_inventory.size();
	view->setInnerContainerSize(Size( 64.0f, innerHeight));

	for (int i = 0; i < _inventory.size(); i++) {
		Item* item = _inventory.at(i);
		auto button = Button::create("use.png");
		CC_ASSERT(button);
		button->addTouchEventListener([this, item](cocos2d::Ref *pSender, Widget::TouchEventType type) {
			if (type != Widget::TouchEventType::ENDED) return;

			auto equipment = dynamic_cast<Equipment*>(item);
			if (equipment) {
				this->equip(equipment);
				CCLOG("equip: %s", equipment->getClassName().c_str());
			}
		});
		item->setPosition(ICON_SIZE / 2.0f, innerHeight - ICON_SIZE / 2.0f - ICON_SIZE*i);
		button->setPosition(Vec2( ICON_SIZE * 3.0f / 2.0f, innerHeight - ICON_SIZE / 2.0f - ICON_SIZE*i));
		view->addChild(item);
		view->addChild(button);
	}

	return view;
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

void You::attack() {
	auto damage = DamageEvent::create(getPosition(), getBoundingBox(), getStrength(), this);
	Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(DamageEvent::getEventName(),
																	   damage);
}

You* You::getInstance() {
	static You* instance = You::create();
	return instance;
}

void You::updateCustom(float dt) {
}