#include "SerializableSprite.h"

using namespace std;

Document SerializableSprite::toJson() const {
	Document doc;
	doc.SetObject();
	return doc;
}


SerializableSprite::SerializableSprite()
{
	_rowid = RandomHelper::random_int(numeric_limits<int64_t>::min(), -1LL);
}