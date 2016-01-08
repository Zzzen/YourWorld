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

bool Statue::initWithJson(const Document& json) {
	if (!SerializableSprite::initWithJson(json)) {
		return false;
	}

	assert(json.HasMember("type")&&json["type"].IsInt());
	auto type = json["type"].GetInt();

	return initWithType(static_cast<Type>(type));
}

Document Statue::toJson() const {
	auto json = SerializableSprite::toJson();

	int type = getType();
	json.AddMember("type", rapidjson::Value(type), json.GetAllocator());

	return json;
}

Statue* Statue::createWithJson(const Document& json) {
	auto statue = new Statue();
	if (statue&&statue->initWithJson(json)) {
		statue->autorelease();
		return statue;
	}
	else {
		CC_SAFE_DELETE(statue);
		return nullptr;
	}
}