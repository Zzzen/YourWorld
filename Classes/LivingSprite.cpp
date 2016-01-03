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

unordered_map<string, string> LivingSprite::toJson() const {
	auto map = SerializableSprite::toJson();
	map["HP"] = to_string(_HP);
	map["age"] = to_string(_age);

	return map;
}