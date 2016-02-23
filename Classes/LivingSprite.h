#pragma once
#include "SerializableSprite.h"
#include "cocostudio\CocoStudio.h"
#include "AnimationConfig.h"
#include "CommonDefinition.h"

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
	CC_SYNTHESIZE(int64_t, _timeOfBirth, TimeOfBirth);

	static const string UPDATE_CUSTOM;

	virtual int getMaxHP() const { return _maxHP; }
protected:
	LivingSprite();

	bool init() override;

	virtual void initPhysicsBody();

	virtual void die() = 0;
	int _maxHP;

	virtual void updateCustom(float dt) = 0;
	virtual float getUpdateInterval()const = 0;

	virtual AnimationConfig getAnimationConfig() const = 0;
	virtual PhysicsBodyConfig getPhysicsBodyConfig() const = 0;

	bool initWithJson(const Document& json) override;

	//position of animationNode is ZERO.
	cocostudio::timeline::SkeletonNode* _skeletalNode;
	//_stateAction is constructed from the same file as skeletalNode
	cocostudio::timeline::ActionTimeline* _stateAction;

private:
	//add skeletal animation if file name is specified.
	void initSkeletalAnimation();
	virtual int getOriginalMaxHP()const = 0;

};
