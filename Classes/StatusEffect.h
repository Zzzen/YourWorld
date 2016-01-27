#pragma once

#include "cocos2d.h"

USING_NS_CC;

class AttackableSprite;

class StatusEffect : public Node {
public:
	//call begin() and schedule update() once 1 second.
	void setTarget(AttackableSprite* target);

	//called when the same effect is added again.
	virtual void refresh();

	virtual std::string getClassName() const = 0;

	inline AttackableSprite* getTarget() { return _target; }

	~StatusEffect() override { CCLOG("~StatusEffect"); }
protected:
	virtual void begin() = 0;
	virtual void end() = 0;

	//this object may be destroyed
	//call this function at the end of reimplemention.
	virtual void updateCustom(float dt);

	float _duration;
	AttackableSprite* _target;
	
	StatusEffect();

private:
	virtual float getMaxDuration() const = 0;
	void updateCustomCaller(float dt) { updateCustom(dt); }
};