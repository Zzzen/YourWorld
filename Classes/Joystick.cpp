#include "Joystick.h"

static const float PI = 3.1415926f;


Joystick::Joystick():
	_background(nullptr),
	_center(nullptr)
{
}

Joystick::~Joystick() {
}

bool Joystick::init() {
	bool result = false;
	do {
		if (!Node::init()) {
			break;
		}

		// joystick的背景
		_background = Sprite::create("joystick_bg.png");
		assert(_background);
		CC_BREAK_IF(!_background);

		_radius = _background->getContentSize().width / 2;
		_centralPos = Vec2(_radius, _radius);

		_background->setPosition(_centralPos);
		addChild(_background);

		setPosition(_centralPos*0.6f);

		// joystick的中心点
		_center = Sprite::create("joystick_center.png");
		assert(_center);
		CC_BREAK_IF(!_center);

		_center->setPosition(_centralPos);
		addChild(_center);

		// touch event监听
		auto touchListener = EventListenerTouchOneByOne::create();
		CC_BREAK_IF(!touchListener);

		touchListener->setSwallowTouches(true);
		touchListener->onTouchBegan =
			CC_CALLBACK_2(Joystick::onTouchBegan, this);
		touchListener->onTouchMoved =
			CC_CALLBACK_2(Joystick::onTouchMoved, this);
		touchListener->onTouchEnded =
			CC_CALLBACK_2(Joystick::onTouchEnded, this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener,
			this);

		result = true;
	} while (0);

	return result;
}

bool Joystick::onTouchBegan(Touch *touch, Event *unused_event) {
	log("onTouchBegan");
	auto point = convertTouchToNodeSpace(touch);
	return _center->getBoundingBox().containsPoint(point);
}

void Joystick::onTouchMoved(Touch *touch, Event *unused_event) {
//	log("onTouchMoved");
	auto point = convertTouchToNodeSpace(touch);

	Vec2 direction(point.x - _centralPos.x, point.y - _centralPos.y);
	if (direction.length() > _radius) {
		direction.normalize();
	}
	else {
		direction = direction / _radius;
	}

	_center->setPosition(direction*_radius + _centralPos);

	_gradientVector = direction;
}

void Joystick::onTouchEnded(Touch *touch, Event *unused_event) {
//	log("onTouchEnded");
	_gradientVector = Vec2::ZERO;
	_center->setPosition(_centralPos);
}