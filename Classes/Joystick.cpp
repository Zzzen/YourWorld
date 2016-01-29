#include "Joystick.h"
#include "Utils.h"

static const float PI = 3.1415926f;


Joystick::Joystick():
	_background(nullptr),
	_center(nullptr),
	_isTouched(false)
{
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

		setPosition(_centralPos*0.6f + Vec2(0, getVisibleSize().height*0.2f));

		// joystick的中心点
		_center = Sprite::create("joystick_center.png");
		assert(_center);
		CC_BREAK_IF(!_center);

		_center->setPosition(_centralPos);
		addChild(_center);


		auto listener = EventListenerTouchAllAtOnce::create();
		listener->onTouchesBegan = CC_CALLBACK_2(Joystick::onTouchesBegan, this);
		listener->onTouchesMoved = CC_CALLBACK_2(Joystick::onTouchesMoved, this);
		listener->onTouchesEnded = CC_CALLBACK_2(Joystick::onTouchesEnded, this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

		result = true;
	} while (0);

	return result;
}



void Joystick::onTouchesBegan(const std::vector<Touch*>& touches, Event * event)
{
	for (auto touch : touches) {
		auto point = convertTouchToNodeSpace(touch);
		if (_center->getBoundingBox().containsPoint(point)) {
			_isTouched = true;
			_touchID = touch->getID();
		}
	}

}

void Joystick::onTouchesMoved(const std::vector<Touch*>& touches, Event * event)
{
	if (!_isTouched) return;

	for (auto touch : touches) {
		// not the touch we care
		if (touch->getID() != _touchID) continue;


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
}

void Joystick::onTouchesEnded(const std::vector<Touch*>& touches, Event * event)
{
	if (!_isTouched) return;

	for (auto touch : touches) {
		// not the touch we care
		if (touch->getID() != _touchID) continue;

		_isTouched = false;
		_touchID = -1;
		_gradientVector = Vec2::ZERO;
		_center->setPosition(_centralPos);
	}
}

Joystick::~Joystick() {
}
