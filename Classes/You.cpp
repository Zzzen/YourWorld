#include "You.h"
#include "DamageEventData.h"
#include "Utils.h"
#include "ui\UIButton.h"
#include "ui\UIWidget.h"
#include "Equipment.h"
#include "Consumable.h"
#include "i18n.h"
#include "TextButton.h"
#include "Utils.h"

void You::setPosition(float x, float y) {

	const Vec2 position(x, y);

	auto previousPosition = getPosition();

	if (previousPosition == position){
		return;
	}

	AttackableSprite::setPosition(position.x, position.y);

	auto moveEvent = YourMoveEvent::createWithWho(this);
	moveEvent->offset = position - previousPosition;
	Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(
		YourMoveEvent::getName(), moveEvent);
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
			case Item::THROWABLE:
				_itemToThrow = dynamic_cast<ThrowableItem*>(item);
				_itemLabel->setTexture(item->getTexture());
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

You::You():
	_itemLabel(nullptr),
	_itemToThrow(nullptr)
{
}

You::~You()
{
	CC_SAFE_RELEASE_NULL(_itemLabel);
}

You* You::getInstance() {
	static You* instance = You::create();
	return instance;
}

bool You::init()
{
	if (!AttackableSprite::init()) { return false; }

	_itemLabel = Sprite::create("defaultItem.png");
	_itemLabel->retain();
	
	Vec2 defaultPos(0.8f * getVisibleSize().width, 0.15f* getVisibleSize().height);
	_itemLabel->setPosition(defaultPos);

	auto throwListener = EventListenerTouchOneByOne::create();
	throwListener->setSwallowTouches(true);
	throwListener->onTouchBegan = [this](Touch* touch, Event*) {
		auto parent = _itemLabel->getParent();
		assert(parent != nullptr);
		auto pos = parent->convertTouchToNodeSpace(touch);
		return _itemLabel->getBoundingBox().containsPoint(pos);
	};
	throwListener->onTouchMoved = [this](Touch* touch, Event*) {
		if(!_itemToThrow) return;

		auto parent = _itemLabel->getParent();
		assert(parent != nullptr);
		auto pos = parent->convertTouchToNodeSpace(touch);
		_itemLabel->setPosition(pos);
	};
	throwListener->onTouchCancelled = [this, throwListener](Touch* touch, Event* event) {
		throwListener->onTouchEnded(touch, event);
	};
	throwListener->onTouchEnded =  [this, defaultPos](Touch* touch, Event*) {
		//getParent of `this`
		auto pos = getParent()->convertTouchToNodeSpace(touch);
		if (_inventory.find(_itemToThrow) != _inventory.end()) {
			_itemToThrow->beThrowed(this, pos);
			_inventory.eraseObject(_itemToThrow);
			_itemToThrow = nullptr;
		}

		_itemLabel->setTexture("defaultItem.png");

		auto moveTo = MoveTo::create(0.1f, defaultPos);
		_itemLabel->runAction(moveTo);
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(throwListener, _itemLabel);
	
	return true;
}

void You::updateCustom(float dt) {
}