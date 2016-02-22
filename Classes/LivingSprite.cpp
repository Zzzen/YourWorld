#include "LivingSprite.h"
#include "MyTime.h"

const string LivingSprite::UPDATE_CUSTOM = "UPDATE_CUSTOM";

LivingSprite::LivingSprite()
	:_skeletalNode(nullptr),
	 _stateAction(nullptr)
{
	_timeOfBirth = MyTime::getInstance()->getRealMesc();
}

void LivingSprite::setHP(int newHP) {
	if (newHP < 1 && _HP >= 1 ) {
		die();
	}

	_HP = newHP;


	if (newHP > _maxHP) {
		_HP = _maxHP;
	}
}

int LivingSprite::getHP() {
	return _HP;
}

Document LivingSprite::toJson() const {
	auto json = SerializableSprite::toJson();

	json.AddMember("HP", rapidjson::Value(_HP), json.GetAllocator());
	json.AddMember("timeOfBirth", rapidjson::Value(_timeOfBirth), json.GetAllocator());

	return json;
}

bool LivingSprite::initWithJson(const Document& json) {
	if (!SerializableSprite::initWithJson(json)) {
		return false;
	}

	assert(json.HasMember("HP") && json["HP"].IsInt());
	assert(json.HasMember("timeOfBirth") && json["timeOfBirth"].IsInt64());
	setHP(json["HP"].GetInt());
	setTimeOfBirth(json["timeOfBirth"].GetInt64());

	return true;
}

void LivingSprite::initSkeletalAnimation() {
	const auto fileName = getAnimationConfig().fileName;
	if (!fileName.empty()) {
		_skeletalNode = dynamic_cast<cocostudio::timeline::SkeletonNode*>(CSLoader::createNode(fileName));
		_stateAction = CSLoader::createTimeline(fileName);
		CCASSERT( _skeletalNode && _stateAction , (fileName + " :failed to initialize skeletal animation.").c_str());
		_skeletalNode->runAction(_stateAction);

//		_skeletalNode->setAnchorPoint(Point::ZERO);
		_skeletalNode->setPosition(Point::ZERO);
		addChild(_skeletalNode);
	}
}

void LivingSprite::startSkeletalAnimation() {
	assert(_stateAction);
	if (_stateAction) {
		_stateAction->gotoFrameAndPlay(0, getAnimationConfig().idleEndFrame, true);
	}
}

void LivingSprite::pauseSkeletalAnimation() {
	if (_stateAction) {
		_stateAction->pause();
	}
}

bool LivingSprite::init() {
	if (!SerializableSprite::init()) {
		return false;
	}

	_maxHP = getOriginalMaxHP();
	setHP(_maxHP);

	schedule([this](float dt) { updateCustom(dt); }, getUpdateInterval(), UPDATE_CUSTOM);
	initSkeletalAnimation();

	return true;
}


Rect LivingSprite::getBoundingBox() const {
	auto rect = _skeletalNode->getBoundingBox();
	return RectApplyAffineTransform(rect, getNodeToParentAffineTransform());
}