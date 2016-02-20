#include "Crop.h"

void Crop::setTouchable(bool enabled)
{
	_touchListener->setEnabled(enabled);
}

bool Crop::init()
{
	if (!LivingSprite::init()) return false;

	_touchListener = EventListenerTouchOneByOne::create();
	_touchListener->setSwallowTouches(true);
	_touchListener->onTouchBegan = [this](Touch* touch, Event*) {
		return _skeletalNode->getBoundingBox().containsPoint(convertTouchToNodeSpace(touch));
	};
	_touchListener->onTouchEnded = [this](Touch*, Event*) {
		if (isPluckable()) {
			bePlucked();
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_touchListener, this);

	return true;
}

void Crop::updateCustom(float dt)
{
}

Crop::Crop():
	_touchListener(nullptr)
{
}

void Crop::onEnter()
{
	LivingSprite::onEnter();
	startSkeletalAnimation();
}
