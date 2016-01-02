#ifndef __JOYSTICK_H__
#define __JOYSTICK_H__

#include "cocos2d.h"

USING_NS_CC;

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
	Sprite* _background;
	Sprite* _center;
	Vec2 _gradientVector;

	double _radius;
	Vec2 _centralPos;
};


#endif