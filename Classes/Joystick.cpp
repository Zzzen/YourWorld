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
	// ȡ���¼�����
	_eventDispatcher->removeEventListenersForTarget(this);
}

bool Joystick::init() {
	bool result = false;
	do {
		// �����ʼ��
		if (!Node::init()) {
			break;
		}
		// joystick�ı���
		mJsBg = Sprite::create("joystick_bg.png");
		assert(mJsBg);
		CC_BREAK_IF(!mJsBg);

		mJsBg->setPosition(mJsPos);
		addChild(mJsBg);

		// joystick�����ĵ�
		mJsCenter = Sprite::create("joystick_center.png");
		assert(mJsCenter);
		CC_BREAK_IF(!mJsCenter);

		mJsCenter->setPosition(mJsPos);
		addChild(mJsCenter);

		// touch event����
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
		// ����������joystick�����ĵ㣬����������¼�
		return true;
	}
	// ���򲻽��ܺ����¼�
	return false;
}

void Joystick::onTouchMoved(Touch *touch, Event *unused_event) {
//	log("onTouchMoved");

	// 1. ��ýǶȣ�
	//��һ������0,90��
	//�ڶ�������90,180��
	//����������-90,-180��
	//����������-90,0��
	Vec2 point = touch->getLocation();
	double y = point.y - mJsPos.y;
	double x = point.x - mJsPos.x;
	double angle = atan2(y, x) * 180 / PI;

	// 2. ����joystick���ĵ�λ�ã�Ŀ�����������ĵ�ʼ�������ı���ͼ��Χ
	// joystick����ͼ�뾶
	double jsBgRadis = mJsBg->getContentSize().width * 0.5;
	//�����㵽���ĵ��ʵ�ʾ���
	double distanceOfTouchPointToCenter = sqrt(
		pow(mJsPos.x - point.x, 2) + pow(mJsPos.y - point.y, 2));
	double deltX=x, deltY=y;
	if (distanceOfTouchPointToCenter >= jsBgRadis) {
		//���õȱȹ�ϵ����delta x y
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
	// �¼���������ԭjoystick���ĵ�λ��
	_gradientVector = Vec2::ZERO;
	mJsCenter->setPosition(mJsPos);
}