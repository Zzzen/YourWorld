#include "Crop.h"
#include "MyTime.h"

void Crop::bePlucked()
{
	_lastPluckedTime = MyTime::getInstance()->getRealMesc();
}

void Crop::setLastPluckedTime(long long time)
{
	_lastPluckedTime = time;
}

void Crop::setTouchable(bool enabled)
{
	_touchListener->setEnabled(enabled);
}

bool Crop::init()
{
	if (!LivingSprite::init()) return false;

	if (getPhysicsBody()) {
		scheduleOnce([this](float) {
			getPhysicsBody()->removeFromWorld();
		}, 0.f, "removePhysicsBody");
	}

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

bool Crop::initWithJson(const Document & json)
{
	if (!LivingSprite::initWithJson(json)) return false;

	assert(json.HasMember("lastPluckedTime") && json["lastPluckedTime"].IsInt64());

	setLastPluckedTime(json["lastPluckedTime"].GetInt64());

	return true;
}

Document Crop::toJson() const
{
	auto json = LivingSprite::toJson();

	json.AddMember("lastPluckedTime", rapidjson::Value(_lastPluckedTime), json.GetAllocator());

	return json;
}

void Crop::updateCustom(float dt)
{
}

Crop::Crop():
	_touchListener(nullptr),
	_lastPluckedTime(0)
{
}

void Crop::onEnter()
{
	LivingSprite::onEnter();
	startSkeletalAnimation();
}
