#ifndef __JOYSTICK_H__
#define __JOYSTICK_H__

#include "cocos2d.h"
#include "FreeArrow.h"

USING_NS_CC;

class Joystick : public FreeArrow {
public:
	//@return a unit vector
	Vec2 getGradientVector();

	CREATE_FUNC(Joystick);

protected:
	bool isPointAccepted(const Point& pos) override;
	bool init() override;
};


#endif