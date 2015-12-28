#ifndef __JOYSTICK_H__
#define __JOYSTICK_H__

#include "cocos2d.h"

USING_NS_CC;

class JoystickEvent : public Ref{
private:
	JoystickEvent();

public:
	virtual ~JoystickEvent();
	virtual bool init();
	CREATE_FUNC(JoystickEvent);

public:
	static const std::string EVENT_JOYSTICK;
	double mAnagle;
};



class Joystick : public Node{
private:
	Joystick();

protected:
	bool onTouchBegan(Touch *touch, Event *unused_event);
	void onTouchMoved(Touch *touch, Event *unused_event);
	void onTouchEnded(Touch *touch, Event *unused_event);

public:
	//@return a unit vector
	inline Vec2 getGradientVector(){
		return _gradientVector;
	}

	virtual ~Joystick();
	virtual bool init();
	CREATE_FUNC(Joystick);

private:
	Sprite* mJsBg;
	Sprite* mJsCenter;
	Vec2 _gradientVector;
	Vec2 mJsPos = Vec2(100, 100);
};


#endif