#include "You.h"
#include "DamageEvent.h"
#include "Utils.h"
#include "ui\UIButton.h"
#include "ui\UIWidget.h"
#include "Equipment.h"
#include "Consumable.h"
#include "i18n.h"

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
	view->setContentSize(Size(getVisibleSize().width * 3.0f / 5.0f, getVisibleSize().height / 2.0f));
	
	const static int ICON_SIZE = 32;
	float innerHeight = ICON_SIZE*_inventory.size();
	view->setInnerContainerSize(Size( getVisibleSize().width * 2.0f / 5.0f, innerHeight));

	for (int i = 0; i < _inventory.size(); i++) {
		Item* item = _inventory.at(i);
		auto detailBtn = Button::create("40X20.png");
		detailBtn->setTitleText(R::getString(R::DETAILS));
		detailBtn->setTitleColor(Color3B(127, 0, 0));

		auto useBtn = Button::create("40X20.png");
		useBtn->setTitleText(R::getString(R::USE));
		useBtn->setTitleColor(Color3B(127, 0, 0));
		CC_ASSERT(useBtn);
		useBtn->addTouchEventListener([this, item, useBtn, detailBtn](cocos2d::Ref *pSender, Widget::TouchEventType type) {
			if (type != Widget::TouchEventType::ENDED) return;

			switch (item->getItemType())
			{
			case Item::CONSUMABLES:
				consume(dynamic_cast<Consumable*>(item));
				useBtn->removeFromParent();
				detailBtn->removeFromParent();
				break;
			case Item::EQUIPMENTS:
				equip(dynamic_cast<Equipment*>(item));
				break;
			case Item::OTHERS:
				break;
			default:
				CC_ASSERT("unexpected Item type");
				break;
			}
		});
		item->setPosition(ICON_SIZE / 2.0f, innerHeight - ICON_SIZE / 2.0f - ICON_SIZE*i);
		useBtn->setPosition(Vec2( ICON_SIZE * 3.0f / 2.0f, innerHeight - ICON_SIZE / 2.0f - ICON_SIZE*i));

		detailBtn->setPosition(Vec2(ICON_SIZE + useBtn->getRightBoundary(), innerHeight - ICON_SIZE / 2.0f - ICON_SIZE*i));  //to do
		detailBtn->addTouchEventListener([this, item, detailBtn](cocos2d::Ref *pSender, Widget::TouchEventType type) {
			if (type != Widget::TouchEventType::ENDED) return;
			string labelName("detail label");
			if (detailBtn->getChildByName(labelName)) return;

			Label* label = Label::create();
			label->setName(labelName);
			label->setString(item->getDetails());
			detailBtn->addChild(label);
			label->setPosition(detailBtn->getBoundingBox().size.width + 8.0f, detailBtn->getBoundingBox().size.height / 2.0f);
			label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
		});

		view->addChild(item);
		view->addChild(useBtn);
		view->addChild(detailBtn);
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