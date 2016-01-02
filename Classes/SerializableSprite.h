#ifndef __SERIALIZABLE_SPRITE_H__
#define __SERIALIZABLE_SPRITE_H__

#include "cocos2d.h"

USING_NS_CC;
using namespace std;

// base class of everything except tmx
class SerializableSprite : public Sprite {
public:
	//stores datum into a map.
	//+ x, y, className
	virtual unordered_map<string, string> toJson();
	virtual string getClassName() = 0;
};




#endif // !__SERIALIZABLE_SPRITE_H__
