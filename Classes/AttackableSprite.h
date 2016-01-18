#pragma once

#include "LivingSprite.h"
#include "ui\UILoadingBar.h"

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

	void setPosition(const Point& pos) override;

	void setHP(int hp) override;

	virtual float getStrength() const = 0;
	virtual float getArmor() const { return 0; }
	virtual float getMoveSpeed() const = 0;

	~AttackableSprite() override;

protected:
	bool init() override;
	bool initWithJson(const Document& json) override;

	virtual void attack() = 0;

	//receive damage event, may not be in range.
	virtual void onAttacked(EventCustom* event);


	//prepare actions from 
	virtual bool initActions();

	SpriteState _state;
	map<SpriteState, cocostudio::timeline::ActionTimeline*> _actions;

	Vec2 _direction;

	AttackableSprite() :_state(IDLE) {}

private:
	EventListenerCustom* _damageListener;
	ProgressTimer* _HPBar;
};