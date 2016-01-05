#include "SpriteManager.h"
#include "Chunk.h"
#include "CommonDefinition.h"
#include "SQLUtils.h"

#define RAPIDJSON_HAS_STDSTRING 1
#include "json\rapidjson.h"
#include "json\document.h"

using namespace std;
using namespace rapidjson;

const static int SPRITE_ZORDER = 100;

void SpriteManager::onChunkCreated(EventCustom* event) {
	const Chunk* chunk = (static_cast<ChunkDisjoinWorldEvent*>(event->getUserData()))->getWho();
	Point pos = chunk->getPosition();

	auto sprites = SQLUtils::selectSprites(pair<int, int>(pos.x, pos.x + Chunk::SIDE_LENGTH*TILE_SIZE),
		pair<int, int>(pos.y, pos.y + Chunk::SIDE_LENGTH*TILE_SIZE));
	
	//initialize sprites and add thems to the layer.
	for (const auto& map : sprites) {
		createSprite(map);
	}

	//if no sprite is retrieved
	bool noSprite = sprites.size() == 0;
}

void SpriteManager::createSprite(const unordered_map<string, string>& map) {
	const int x = stoi(map.at("x")),
		      y = stoi(map.at("y"));
	const string className = map.at("className"),
                 properties = map.at("properties");

	SerializableSprite* sprite = nullptr;
	if (className == "Statue") {
		sprite = createStatue(properties);
	}



	if (!sprite) {
		return;
	}

	sprite->setPosition(x, y);
	_sprites.pushBack(sprite);
	_layer->addChild(sprite, SPRITE_ZORDER);

	log("SpriteManager::createSprite: x: %d, y: %d, class: %s, properties: %s",
		x, y, className.c_str(), properties.c_str());
}

Statue* SpriteManager::createStatue(string json) {
	Document d;
	d.Parse(json.c_str());

	string type = d["type"].GetString();
	int t = stoi(type);
	return Statue::createWithType(static_cast<Statue::Type>(t));
}