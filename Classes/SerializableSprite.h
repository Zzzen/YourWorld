#ifndef __SERIALIZABLE_SPRITE_H__
#define __SERIALIZABLE_SPRITE_H__

#include "cocos2d.h"

#define RAPIDJSON_HAS_STDSTRING 1
#include "json/document.h"

USING_NS_CC;
using namespace std;
using namespace rapidjson;

// base class of everything except tmx
class SerializableSprite : public Sprite {
public:
	//stores datum into a json document.
	virtual Document toJson() const;
	virtual const string getClassName() const = 0;

protected:
	virtual bool initWithJson(const Document& json) { return true; }
};




#endif // !__SERIALIZABLE_SPRITE_H__
