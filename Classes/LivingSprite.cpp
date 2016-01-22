#include "LivingSprite.h"

LivingSprite::LivingSprite()
	:_skeletalNode(nullptr),
	 _idleAction(nullptr)
{
}

void LivingSprite::setHP(int newHP) {
	if (newHP < 1) {
		die();
	}

	_HP = newHP;
}

int LivingSprite::getHP() {
	return _HP;
}

Document LivingSprite::toJson() const {
	auto json = SerializableSprite::toJson();

	json.AddMember("HP", rapidjson::Value(_HP), json.GetAllocator());
	json.AddMember("age", rapidjson::Value(_age), json.GetAllocator());

	return json;
}

bool LivingSprite::initWithJson(const Document& json) {
	if (!SerializableSprite::initWithJson(json)) {
		return false;
	}

	schedule(CC_SCHEDULE_SELECTOR(LivingSprite::updateCustom), getUpdateInterval(), kRepeatForever, 0);

	initSkeletalAnimation();

	assert(json.HasMember("HP") && json["HP"].IsInt());
	assert(json.HasMember("age") && json["age"].IsInt());
	setHP(json["HP"].GetInt());
	setAge(json["age"].GetInt());

	return true;
}

void LivingSprite::initSkeletalAnimation() {
	const auto fileName = getSkeletalFileName();
	if (!fileName.empty()) {
		_skeletalNode = CSLoader::createNode(fileName);
		_idleAction = CSLoader::createTimeline(fileName);
		CCASSERT( _skeletalNode && _idleAction , (fileName + " :failed to initialize skeletal animation.").c_str());
		_skeletalNode->runAction(_idleAction);

//		_skeletalNode->setAnchorPoint(Point::ZERO);
		_skeletalNode->setPosition(Point::ZERO);
		addChild(_skeletalNode);
	}
}

void LivingSprite::startSkeletalAnimation() {
	if (_idleAction) {
		_idleAction->gotoFrameAndPlay(0, true);
	}
}

void LivingSprite::pauseSkeletalAnimation() {
	if (_idleAction) {
		_idleAction->pause();
	}
}

bool LivingSprite::init() {
	if (!SerializableSprite::init()) {
		return false;
	}

	_maxHP = getOriginalMaxHP();
	setHP(_maxHP);

	schedule(CC_SCHEDULE_SELECTOR(LivingSprite::updateCustom), getUpdateInterval(), kRepeatForever, 0);
	initSkeletalAnimation();

	return true;
}


Rect LivingSprite::getBoundingBox() const {
	auto rect = _skeletalNode->getBoundingBox();
	return RectApplyAffineTransform(rect, getNodeToParentAffineTransform());
}