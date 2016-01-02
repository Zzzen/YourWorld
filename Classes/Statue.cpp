#include "Statue.h"

Statue* Statue::createWithType(Type type) {
	auto statue = new (std::nothrow)  Statue();
	if (statue && statue->initWithType(type)) {
		statue->autorelease();
		return statue;
	}
	else {
		CC_SAFE_DELETE(statue);
		return nullptr;
	}
}

Statue::Statue() {}

bool Statue::initWithType(Type type) {
	setType(type);

	std::string filename;
	switch (type)
	{
	case Statue::SPEED:
		filename = "speed_statue.png";
		break;
	case Statue::HEAL:
		filename = "heal_statue.png";
		break;
	default:
		break;
	}

	return initWithFile(filename);
}

unordered_map<string, string> Statue::toJson() {
	auto map = SerializableSprite::toJson();

	map["type"] = to_string(getType());
	map["class"] = "Statue";

	return map;
}