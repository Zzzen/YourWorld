#include "Skeleton.h"
#include "Jian.h"

void Skeleton::attack()
{
	if (!_attackTarget) {
		return;
	}

	auto deltaVec = _attackTarget->getPosition() - getPosition();

	auto j = Jian::create(deltaVec.getAngle(), 100, 3, 10, this);
	j->setPosition(getPosition());
	j->start();

	getParent()->addChild(j);
}

Skeleton::Skeleton():
	_weaponNode(nullptr)
{
}

bool Skeleton::init()
{
	if (!RangedAttackMob::init()) {

		return false;
	}

	enumerateChildren("//weapon", [this](Node* node) {
		_weaponNode = node;
		return true;
	});

	assert(_weaponNode);

	return true;
}

void Skeleton::updateCustom(float dt)
{
	RangedAttackMob::updateCustom(dt);

	if (_attackTarget && _weaponNode) {
		auto direction = _attackTarget->getPosition() - getPosition();
		_weaponNode->setRotation(-MATH_RAD_TO_DEG(direction.getAngle()));
	}
}


//void init() {
//
//	auto timelines = _stateAction->getTimelines();
//	auto children = getChildren();
//
//	Node* cao1 = nullptr;
//	enumerateChildren("//cao_1", [&cao1](Node* node) {
//		cao1 = node;
//		return true;
//	});
//
//	assert(cao1);
//
//	USING_NS_TIMELINE;
//	using namespace cocostudio;
//
//
//	auto data = dynamic_cast<ComExtensionData*>(cao1->getComponent("ComExtensionData"));
//	assert(data);
//
//	for (auto& timeline : timelines) {
//		if (timeline->getActionTag() != data->getActionTag()) {
//			continue;
//		}
//
//		auto frames = timeline->getFrames();
//	}
//}