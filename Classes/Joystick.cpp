#include "Joystick.h"

static const float PI = 3.1415926f;

const std::string JoystickEvent::EVENT_JOYSTICK = "event_of_joystick";
JoystickEvent::JoystickEvent() {
//	log(" JoystickEvent(), %p", this);
}

JoystickEvent::~JoystickEvent() {
//	log("~JoystickEvent(), %p", this);
}

bool JoystickEvent::init() {
	return true;
}

Joystick::Joystick() {
}

Joystick::~Joystick() {
	// 取消事件监听
	_eventDispatcher->removeEventListenersForTarget(this);
}

bool Joystick::init() {
	bool result = false;
	do {
		// 父类初始化
		if (!Node::init()) {
			break;
		}
		// joystick的背景
		mJsBg = Sprite::create("joystick_bg.png");
		assert(mJsBg);
		CC_BREAK_IF(!mJsBg);

		mJsBg->setPosition(mJsPos);
		addChild(mJsBg);

		// joystick的中心点
		mJsCenter = Sprite::create("joystick_center.png");
		assert(mJsCenter);
		CC_BREAK_IF(!mJsCenter);

		mJsCenter->setPosition(mJsPos);
		addChild(mJsCenter);

		// touch event监听
		auto touchListener = EventListenerTouchOneByOne::create();
		if (nullptr == touchListener) {
			break;
		}
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
//	log("onTouchBegan");
	auto point = touch->getLocation();
	if (mJsCenter->getBoundingBox().containsPoint(point)) {
		// 若触摸点在joystick的中心点，则继续接受事件
		return true;
	}
	// 否则不接受后续事件
	return false;
}

void Joystick::onTouchMoved(Touch *touch, Event *unused_event) {
//	log("onTouchMoved");

	// 1. 获得角度，
	//第一象限是0,90度
	//第二象限是90,180度
	//第三象限是-90,-180度
	//第四象限是-90,0度
	Vec2 point = touch->getLocation();
	double y = point.y - mJsPos.y;
	double x = point.x - mJsPos.x;
	double angle = atan2(y, x) * 180 / PI;

	// 2. 更新joystick中心点位置，目的是想让中心点始终在它的背景图范围
	// joystick背景图半径
	double jsBgRadis = mJsBg->getContentSize().width * 0.5;
	//触摸点到中心点的实际距离
	double distanceOfTouchPointToCenter = sqrt(
		pow(mJsPos.x - point.x, 2) + pow(mJsPos.y - point.y, 2));
	double deltX=x, deltY=y;
	if (distanceOfTouchPointToCenter >= jsBgRadis) {
		//利用等比关系计算delta x y
		deltX = x * (jsBgRadis / distanceOfTouchPointToCenter);
		deltY = y * (jsBgRadis / distanceOfTouchPointToCenter);
		mJsCenter->setPosition(Vec2(mJsPos.x + deltX, mJsPos.y + deltY));
	}
	else {
		mJsCenter->setPosition(point);
	}


	Vec2 gradient(x, y);
	gradient.normalize();
	_gradientVector = gradient;

}

void Joystick::onTouchEnded(Touch *touch, Event *unused_event) {
//	log("onTouchEnded");
	// 事件结束，还原joystick中心点位置
	_gradientVector = Vec2::ZERO;
	mJsCenter->setPosition(mJsPos);
}