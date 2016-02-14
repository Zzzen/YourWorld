#include "You.h"
#include "DamageEvent.h"
#include "Utils.h"
#include "ui\UIButton.h"
#include "ui\UIWidget.h"
#include "Equipment.h"
#include "Consumable.h"
#include "i18n.h"
#include "TextButton.h"

void You::setPosition(float x, float y) {

	const Vec2 position(x, y);

	auto previousPosition = getPosition();

	if (previousPosition == position){
		return;
	}

	auto moveEvent = YourMoveEvent::createWithWho(this);
	moveEvent->offset = position - previousPosition;
	Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(
		YourMoveEvent::getName(), moveEvent);

	AttackableSprite::setPosition(position.x, position.y);
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
		auto detailBtn = TextButton::create();
		detailBtn->setText(R::getString(R::DETAILS));
		detailBtn->setTextColor(Color4B::MAGENTA);

		auto useBtn = TextButton::create();
		useBtn->setText(R::getString(R::USE));
		useBtn->setTextColor(Color4B::MAGENTA);
		CC_ASSERT(useBtn);
		useBtn->onTouched = [this, item, useBtn, detailBtn, view]() {
			
			switch (item->getItemType())
			{
			case Item::CONSUMABLES:
				consume(dynamic_cast<Consumable*>(item));
				for (Node* child : view->getChildren()) {
					if (child->getPosition().y > item->getPosition().y)
						child->setPosition(child->getPosition() - Vec2(0, ICON_SIZE));
				}
				useBtn->removeFromParent();
				detailBtn->removeFromParent();
				item->removeFromParent();
				view->setInnerContainerSize(view->getInnerContainerSize() - Size(0, ICON_SIZE));
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
		};
		item->setPosition(ICON_SIZE / 2.0f, innerHeight - ICON_SIZE / 2.0f - ICON_SIZE*i);
		useBtn->setPosition(Vec2( ICON_SIZE * 3.0f / 2.0f, innerHeight - ICON_SIZE / 2.0f - ICON_SIZE*i));

		detailBtn->setPosition(Vec2(ICON_SIZE + useBtn->getBoundingBox().getMaxX(), innerHeight - ICON_SIZE / 2.0f - ICON_SIZE*i));  //to do
		detailBtn->onTouched = [this, item, detailBtn]() {

			string labelName("detail label");
			auto label = dynamic_cast<Label*>(detailBtn->getChildByName(labelName));
			if (label) {
				label->setVisible(!label->isVisible());
				return;
			}

			label = Label::create();
			label->setName(labelName);
			label->setString(item->getDetails());
			detailBtn->addChild(label);
			label->setPosition(detailBtn->getBoundingBox().size.width + 8.0f, 0);
			label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);

		};

		view->addChild(item);
		view->addChild(useBtn);
		view->addChild(detailBtn);
	}

	return view;
}

bool You::initWithJson(const Document & json)
{
	if (!AttackableSprite::initWithJson(json)) {
		return false;
	}

	return true;
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

You* You::getInstance() {
	static You* instance = You::create();
	return instance;
}

void You::updateCustom(float dt) {
}