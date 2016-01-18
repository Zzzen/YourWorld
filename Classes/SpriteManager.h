#pragma once

#include "cocos2d.h"
#include "SerializableSprite.h"
#include "Statue.h"


USING_NS_CC;
using namespace std;

class AttackableSprite;
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

	static SpriteManager* getInstance();
	void setLayer(Node* node) { _layer = node; }

	~SpriteManager() {}

	//create sprite and add it to _layer
	AttackableSprite* createSprite(const string& name);

	void registerCreateFunc(const string& name, function<SerializableSprite* ()> func) {
		_createFuncs.insert(pair<string, function<SerializableSprite* ()>>(name, func)); 
	}
private:
	SpriteManager() {}
	void createSprite(const unordered_map<string, string>& map);
	void createNewSprites(const Chunk * chunk);

	//add sprite to game layer and vector
	inline void addSprite(SerializableSprite* sprite);
	inline void removeSprite(SerializableSprite* sprite);

	Node* _layer;
	Vector<SerializableSprite*> _sprites;
	map < string, function< SerializableSprite* ()> > _createFuncs;
};
