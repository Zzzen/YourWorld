#ifndef __SERIALIZABLE_SPRITE_H__
#define __SERIALIZABLE_SPRITE_H__

#include "cocos2d.h"

#define RAPIDJSON_HAS_STDSTRING 1
#include "json/document.h"

USING_NS_CC;
using namespace std;
using namespace rapidjson;

#define CREATE_WITH_JSON_FUNC(Type) static Type* createWithJson(const Document& json){      \
									    auto object = Type::create();						\
										if (object && object->initWithJson(json)) {			\
											return object;									\
										}else{												\
											CC_SAFE_DELETE(object);							\
											return nullptr;									\
										}													\
									}


// base class of everything except tmx
class SerializableSprite : public Sprite {
public:
	//stores datum into a json document.
	virtual Document toJson() const;
	virtual string getClassName() const = 0;

	SerializableSprite();

	void setRowid(int64_t id) { _rowid = id; }

	int64_t getRowid() const { return _rowid; }

protected:
	virtual bool initWithJson(const Document& json) { return true; }

	//since rowid is always a 64-bit signed integer in sqlite3.
	int64_t _rowid;
};




#endif // !__SERIALIZABLE_SPRITE_H__
