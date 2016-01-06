#pragma once

#include "cocos2d.h"
#include "SerializableSprite.h"
#include "Statue.h"

USING_NS_CC;

class Chunk;
class SpriteManager 
{
public:
	//retrieve sprites from sql and add them to the chunk.
	//if no sprite is found, maybe create new sprites.
	void onChunkCreated(EventCustom* event);

	//store and remove sprites on the chunk.
	void onChunkRemoved(EventCustom* event);

	SpriteManager(Layer* layer):_layer(layer) {}
	~SpriteManager() {}

private:
	void createSprite(const unordered_map<string, string>& map);
	void createNewSprites(const Chunk * chunk);

	//add sprite to game layer and vector
	inline void addSprite(SerializableSprite* sprite);

	Layer* _layer;
	Vector<SerializableSprite*> _sprites;
	Statue* createStatue(const string& json);
};
