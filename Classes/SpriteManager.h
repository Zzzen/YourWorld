#pragma once

#include "cocos2d.h"
#include "SerializableSprite.h"
#include "Statue.h"

USING_NS_CC;

class SpriteManager 
{
public:
	void onChunkCreated(EventCustom* event);

	SpriteManager(Layer* layer):_layer(layer) {}
	~SpriteManager() {}

private:
	void createSprite(const unordered_map<string, string>& map);
	Layer* _layer;
	Vector<SerializableSprite*> _sprites;
	Statue* createStatue(string json);
};
