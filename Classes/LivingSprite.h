#pragma once
#include "SerializableSprite.h"
#include "cocostudio\CocoStudio.h"

class LivingSprite: public SerializableSprite
{
public:
	~LivingSprite() override {}

	//+HP, age
	Document toJson() const override;


	void startSkeletalAnimation();
	void pauseSkeletalAnimation();

	CC_PROPERTY(int, _HP, HP);
	CC_SYNTHESIZE(int, _age, Age);

protected:
	LivingSprite();

	bool init() override;

	virtual void die() = 0;

	virtual void updateCustom(float dt) = 0;
	virtual float getUpdateInterval() = 0;

	virtual string getSkeletalFileName() const { return ""; }

	bool initWithJson(const Document& json);

	//anchor and position of animationNode are ZERO.
	Node* _skeletalNode;
	cocostudio::timeline::ActionTimeline* _timeline;


private:

	//add skeletal animation if file name is specified.
	void initSkeletalAnimation();

	void callUpdateCustom(float dt) { updateCustom(dt); }
};
