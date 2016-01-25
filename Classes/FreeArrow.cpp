#include "FreeArrow.h"
#include "Utils.h"

static const float MAX_LENGTH = 40.0f;

FreeArrow * FreeArrow::create()
{
	auto arrow = new FreeArrow();
	if (arrow&&arrow->init()) {
		arrow->autorelease();
		return arrow;
	}
	else {
		CC_SAFE_DELETE(arrow);
		return nullptr;
	}
}

bool FreeArrow::init()
{
	if (!Node::init()) return false;

	_arrow = Sprite::create("arrow.png");
	CC_ASSERT(_arrow);

	_arrow->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	addChild(_arrow);
	_arrow->setVisible(false);

	setPosition(Vec2::ZERO);

	_touchListener = EventListenerTouchOneByOne::create();

	_touchListener->setSwallowTouches(true);
	_touchListener->onTouchBegan =
		CC_CALLBACK_2(FreeArrow::onTouchBegan, this);
	_touchListener->onTouchMoved =
		CC_CALLBACK_2(FreeArrow::onTouchMoved, this);
	_touchListener->onTouchEnded =
		CC_CALLBACK_2(FreeArrow::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_touchListener,
		this);

	return true;
}


bool FreeArrow::onTouchBegan(Touch * touch, Event * unused_event)
{
	auto point = convertTouchToNodeSpace(touch);

	//click on the left part of screen
	if (getVisibleSize().width*0.382 > point.x) return false;

	_arrow->setPosition(point);
	_arrow->setVisible(true);
	_arrow->setScale(0.0f);

	return true;
}

void FreeArrow::onTouchMoved(Touch * touch, Event * unused_event)
{
	auto point = convertTouchToNodeSpace(touch);
	auto dv = point - _arrow->getPosition();
	auto scale = dv.length() / (_arrow->getTexture()->getContentSize().height);
	if (scale > 1.0f) scale = 1.0f;

	_arrow->setRotation(MATH_RAD_TO_DEG( -dv.getAngle() ) + 90.0f);
	_arrow->setScale(scale);
}

void FreeArrow::onTouchEnded(Touch * touch, Event * unused_event)
{
	_arrow->setVisible(false);
}

FreeArrow::FreeArrow():
	_touchListener(nullptr),
	_arrow(nullptr)
{
}
