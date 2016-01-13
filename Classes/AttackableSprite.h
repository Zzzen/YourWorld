#pragma once

#include "LivingSprite.h"

class DamageEvent;
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

	// start play attack animation
	virtual void startAttacking() { if( ATTACK!=_state && FREEZED!=_state) setCurrentState(ATTACK); }
	//receive damage event, may not be in range.
	virtual void onAttacked(EventCustom* event);

	void setPosition(const Point& pos) override;

	~AttackableSprite() override;

protected:
	bool init() override;
	bool initWithJson(const Document& json) override;

	virtual void attack() = 0;
	EventListenerCustom* _damageListener;

	//prepare actions from 
	virtual bool initActions();

	SpriteState _state;
	map<SpriteState, cocostudio::timeline::ActionTimeline*> _actions;

	Vec2 _direction;
};