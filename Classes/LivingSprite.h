#pragma once
#include "SerializableSprite.h"
#include "cocostudio\CocoStudio.h"
#include "AnimationConfig.h"

class LivingSprite: public SerializableSprite
{
public:
	~LivingSprite() override {}

	//+HP, age
	Document toJson() const override;

	//redirect to skeleton node.
	Rect getBoundingBox() const override;

	void startSkeletalAnimation();
	void pauseSkeletalAnimation();

	virtual void addHP(int hp) { setHP(getHP() + hp); }
	CC_PROPERTY(int, _HP, HP);
	CC_SYNTHESIZE(int, _age, Age);

	static const string UPDATE_CUSTOM;

	virtual int getMaxHP() const { return _maxHP; }
protected:
	LivingSprite();

	bool init() override;

	virtual void die() = 0;
	int _maxHP;

	virtual void updateCustom(float dt) = 0;
	virtual float getUpdateInterval() = 0;

	virtual AnimationConfig getAnimationConfig() const = 0;

	bool initWithJson(const Document& json) override;

	//position of animationNode is ZERO.
	Node* _skeletalNode;
	//_stateAction is constructed from the same file as skeletalNode
	cocostudio::timeline::ActionTimeline* _stateAction;

private:
	//add skeletal animation if file name is specified.
	void initSkeletalAnimation();
	virtual int getOriginalMaxHP()const = 0;

};
