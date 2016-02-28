#include "Seed.h"
#include "AttackableSprite.h"
#include "Crop.h"

static float DURATION = 2.f;

void Seed::beThrowed(AttackableSprite * owner, const Vec2 & dst)
{
	_pickable = false;

	setPosition(owner->getPosition());

	auto action = MoveTo::create(DURATION, dst);

	assert(getParent() == nullptr);
	owner->getParent()->addChild(this, owner->getLocalZOrder());

	runAction(action);

	scheduleOnce([this](float) {
		auto crop = createCrop();
		CCASSERT(crop, "fail to create crop");
		if (crop) {
			getParent()->addChild(crop, getLocalZOrder());
			crop->setPosition(getPosition());
		}
		removeFromParent();
	}, action->getDuration(), "createCrop");
}
