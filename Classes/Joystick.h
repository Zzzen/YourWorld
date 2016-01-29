#ifndef __JOYSTICK_H__
#define __JOYSTICK_H__

#include "cocos2d.h"

USING_NS_CC;

class Joystick : public Node{
public:
	//@return a unit vector
	inline Vec2 getGradientVector() {
		return _gradientVector;
	}

	virtual ~Joystick();
	virtual bool init();
	CREATE_FUNC(Joystick);

protected:
	void onTouchesBegan(const std::vector<Touch*>& touches, Event  *event);
	void onTouchesMoved(const std::vector<Touch*>& touches, Event  *event);
	void onTouchesEnded(const std::vector<Touch*>& touches, Event  *event);
	void onTouchesCancelled(const std::vector<Touch*>& touches, Event  *event) {	onTouchesEnded(touches, event);	}

	Joystick();

	Sprite* _background;
	Sprite* _center;
	Vec2 _gradientVector;
	int _touchID;
	bool _isTouched;

	double _radius;
	Vec2 _centralPos;
};


#endif