#pragma once

#include "cocos2d.h"
#include "SerializableSprite.h"
#include "AttackableSprite.h"
#include "Statue.h"

#define RAPIDJSON_HAS_STDSTRING 1
#include "json\rapidjson.h"
#include "json\document.h"

USING_NS_CC;
using namespace std;

class Chunk;
class SpriteManager 
{
public:
	//retrieve sprites from sql and add them to the chunk.
	//if no sprite is found, maybe create new sprites.
	void onChunkCreated(EventCustom* event);

	//store and remove sprites on the chunk.
	void onChunkRemoved(EventCustom* event);

	void onMobDied(EventCustom* event);

	//pick items
	void onYourMove(EventCustom* event);

	static SpriteManager* getInstance();
	void setLayer(Node* node) { _layer = node; }

	//sprites of _layer
	Vector<SerializableSprite*> getAllSprites();

	//
	Vector<AttackableSprite*> getVas() const { return _vas; }
	void updateVas(float dt);

	~SpriteManager() {}

	//create sprite and add it to _layer
	SerializableSprite* createSprite(const string& className);

	SerializableSprite* createSpriteWithJson(const string& className, const Document& doc);

	void registerCreateFunc(const string& className, function<SerializableSprite* ()> func) {
		CCASSERT(className.size() != 0, "className must not be empty");
		_createFuncs.insert(pair<string, function<SerializableSprite* ()>>(className, func));
	}
	void registerCreateFuncWithJson(const string& className, function< SerializableSprite* (const Document&)> func) {
		CCASSERT(className.size() != 0, "className must not be empty");
		_createFuncsWithJson.insert(pair<string, function< SerializableSprite* (const Document&)>>(className, func));
	}
private:
	SpriteManager() {}
	SerializableSprite* createSprite(const unordered_map<string, string>& map);
	void createNewSprites(const Chunk * chunk);

	Vector<AttackableSprite*> _vas;

	Node* _layer;
	map < string, function< SerializableSprite* ()> > _createFuncs;
	map < string, function< SerializableSprite* (const Document&)> > _createFuncsWithJson;
};
