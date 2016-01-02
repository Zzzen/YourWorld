#include "SerializableSprite.h"

using namespace std;

unordered_map<string, string> SerializableSprite::toJson() {
	unordered_map<string, string> map;

	auto pos = getPosition();

	map["x"] = to_string(pos.x);
	map["y"] = to_string(pos.y);
	map["class"] = "SerializableSprite";

	return map;
}