#include "SerializableSprite.h"

using namespace std;

Document SerializableSprite::toJson() const {
	Document doc;
	doc.SetObject();
	return doc;
}