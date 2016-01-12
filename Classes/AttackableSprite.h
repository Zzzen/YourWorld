#pragma once

#include "LivingSprite.h"

class AttackableSprite : public LivingSprite {
public:
	enum SpriteState
	{
		IDLE = 1,
		FOLLOW = 2,
		ATTACK = 3,
		FLEE = 4,
		FREEZED = 5
	};

	//+= state
	Document toJson() const override;

	//and play animation.
	virtual void setCurrentState(SpriteState state);

	SpriteState getCurrentState() const { return _state; }

	virtual void startAttacking() { setCurrentState(ATTACK); }


	~AttackableSprite() override;

protected:
	bool init() override;
	bool initWithJson(const Document& json) override;

	virtual void attack() = 0;

	//prepare actions from 
	virtual bool initActions();

	SpriteState _state;
	map<SpriteState, cocostudio::timeline::ActionTimeline*> _actions;
};