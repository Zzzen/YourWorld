#pragma once

#include "LivingSprite.h"
#include "ui\UILoadingBar.h"
#include "Item.h"

class Equipment;
class DamageEvent;
class AttackableSprite : public LivingSprite {
public:
	enum SpriteState{
		IDLE = 1,
		FOLLOW = 2,
		ATTACK = 3,
		FLEE = 4,
		FREEZED = 5
	};

	enum EquipmentType {
		WEAPON = 1,
		HELMET = 2,
		CHESTPLATE = 3,
		LEGGINS = 4,
		BOOTS = 5
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

	void pick(Item* item) { _inventory.pushBack(item); item->removeFromParent(); }

	~AttackableSprite() override;

	virtual float getStrength() const { return _originalStrength + _extraStrength; }
	virtual float getArmor() const { return _orginalArmor + _extraArmor; }
	virtual float getMoveSpeed() const { return _originalMoveSpeed + _extraMoveSpeed; }

	virtual void addStrength(float s) { _extraStrength += s; }
	virtual void addArmor(float a) { _extraArmor += a; }
	virtual void addMoveSpeed(float m) { _extraMoveSpeed += m; }

protected:
	bool init() override;
	bool initWithJson(const Document& json) override;


	float _originalStrength;
	float _extraStrength;
	float _orginalArmor;
	float _extraArmor;
	float _originalMoveSpeed;
	float _extraMoveSpeed;

	void equip(Equipment* equip);
	virtual void attack() = 0;

	//receive damage event, may not be in range.
	virtual void onAttacked(EventCustom* event);

	map<EquipmentType, Equipment*> _equipments;
	Vector<Item*> _inventory;

	//prepare actions from 
	virtual bool initActions();

	SpriteState _state;
	map<SpriteState, cocostudio::timeline::ActionTimeline*> _actions;

	Vec2 _direction;

	AttackableSprite();

private:
	ProgressTimer* _HPBar;

	virtual float getOriginalStrength() const = 0;
	virtual float getOriginalArmor() const { return 0; }
	virtual float getOriginalMoveSpeed() const = 0;
};