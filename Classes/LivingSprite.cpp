#include "LivingSprite.h"

void LivingSprite::setHP(int newHP) {
	if (newHP < 0) {
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

	schedule(CC_SCHEDULE_SELECTOR(LivingSprite::callUpdateCustom), getUpdateInterval(), kRepeatForever, 0);

	assert(json.HasMember("HP"));
	assert(json.HasMember("age"));
	setHP(json["HP"].GetInt());
	setAge(json["age"].GetInt());

	return true;
}