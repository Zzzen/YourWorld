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

	_arrow->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	addChild(_arrow);
	_arrow->setVisible(false);

	_drawNode = DrawNode::create();
	_arrow->addChild(_drawNode);
	_drawNode->setPosition(Vec2::ZERO);
	_circleColor = Color4F::RED;

	setPosition(Vec2::ZERO);

	_touchListener = EventListenerTouchAllAtOnce::create();

	_touchListener->onTouchesBegan =
		CC_CALLBACK_2(FreeArrow::onTouchesBegan, this);
	_touchListener->onTouchesMoved =
		CC_CALLBACK_2(FreeArrow::onTouchesMoved, this);
	_touchListener->onTouchesEnded =
		CC_CALLBACK_2(FreeArrow::onTouchesEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_touchListener,
		this);

	return true;
}

bool FreeArrow::isPointAccepted(const Point& pos) {
	return getVisibleSize().width*0.5f < pos.x;
}

void FreeArrow::onTouchesBegan(const std::vector<Touch*>& touches, Event * event)
{
	for (auto touch : touches) {
		auto point = convertTouchToNodeSpace(touch);

		//click on the left part of screen
		if (isPointAccepted(point)) {
			_isTouched = true;
			_touchID = touch->getID();

			_arrow->setPosition(point);
			_arrow->setVisible(true);
			_arrow->setScale(0.0f);

			break;
		}
	}
}

void FreeArrow::onTouchesMoved(const std::vector<Touch*>& touches, Event * event)
{
	if (!_isTouched) return;

	for (auto touch : touches) {
		if (touch->getID() == _touchID) {
			auto point = convertTouchToNodeSpace(touch);
			auto dv = point - _arrow->getPosition();
			auto scale = dv.length() / (_arrow->getTexture()->getContentSize().width);
			if (scale > 1.0f) scale = 1.0f;

			_arrow->setRotation(MATH_RAD_TO_DEG(-dv.getAngle()));
			_arrow->setScale(scale);

			_drawNode->clear();

			_drawNode->drawCircle(Vec2::ZERO, _arrow->getTexture()->getContentSize().width / scale, 0, 7, false, _circleColor);
			break;
		}
	}
}

void FreeArrow::onTouchesEnded(const std::vector<Touch*>& touches, Event * event)
{
	if (!_isTouched) return;

	for (auto touch : touches) {
		if (touch->getID() == _touchID) {
			_isTouched = false;
			_touchID = 0;

			_arrow->setVisible(false);
			_drawNode->clear();

			if (onTouchEnded) {
				onTouchEnded(Vec2::forAngle(MATH_DEG_TO_RAD( - _arrow->getRotation())) * _arrow->getScale());
			}

			break;
		}
	}
}


FreeArrow::FreeArrow():
	_touchListener(nullptr),
	_arrow(nullptr),
	_drawNode(nullptr),
	_isTouched(false),
	onTouchEnded(nullptr)
{
}
