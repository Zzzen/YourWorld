#include "Statue.h"
#include "json/document.h"

using namespace rapidjson;

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

bool Statue::initWithJson(const string& json) {
	Document doc;
	doc.Parse(json.c_str());

	auto type = doc["type"].GetInt();

	return initWithType(static_cast<Type>(type));
}

unordered_map<string, string> Statue::toJson() const {
	auto map = SerializableSprite::toJson();
	map["type"] = to_string(getType());

	return map;
}