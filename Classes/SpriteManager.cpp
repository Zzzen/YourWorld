#include "SpriteManager.h"
#include "Chunk.h"
#include "CommonDefinition.h"
#include "SQLUtils.h"
#include "Human.h"
#include "You.h"

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
	if (noSprite) {
		createNewSprites(chunk);
	}
}

void SpriteManager::onChunkRemoved(EventCustom* event) {
	const Chunk* chunk = (static_cast<ChunkDisjoinWorldEvent*>(event->getUserData()))->getWho();
	auto box = chunk->getBoundingBox();

	std::vector<SerializableSprite*> invisible;
	for (auto sprite : _sprites) {
		if (box.containsPoint(sprite->getPosition())) {
			invisible.push_back(sprite);
		}
	}
	for (auto sprite : invisible) {
		SQLUtils::insertSprite(sprite);
		sprite->removeFromParent();
		_sprites.eraseObject(sprite);
	}
}

void SpriteManager::onMobDied(EventCustom * event)
{
	Mob* mob = (static_cast<MobDieEvent*>(event->getUserData()))->getWho();
	removeSprite(mob);
	auto dabaojian = createSprite("Dabaojian");
	dabaojian->setPosition(mob->getPosition());
}

void SpriteManager::createSprite(const unordered_map<string, string>& map) {
	const int x = stoi(map.at("x")),
		      y = stoi(map.at("y"));
	const string className = map.at("className"),
                 properties = map.at("properties");

	Document json;
	json.Parse(properties.c_str());

	SerializableSprite* sprite = nullptr;
	if (className == "Statue") {
		sprite = Statue::createWithJson(json);
	}



	if (!sprite) {
		log("unable to instantiate sprite. class name: %s, properties: %s",
			className.c_str(), properties.c_str());
		return;
	}

	sprite->setPosition(x, y);

	addSprite(sprite);

	log("SpriteManager::createSprite: x: %d, y: %d, class: %s, properties: %s",
		x, y, className.c_str(), properties.c_str());
}

void SpriteManager::addSprite(SerializableSprite* sprite) {
	_sprites.pushBack(sprite);
	_layer->addChild(sprite, SPRITE_ZORDER);
}

void SpriteManager::removeSprite(SerializableSprite* sprite) {
	_sprites.eraseObject(sprite);
	sprite->removeFromParent();
}


void SpriteManager::createNewSprites(const Chunk* chunk) {
	auto vecs = chunk->getGradientVectors();
	float a1 = vecs.bottomLeft.getAngle(),
		a2 = vecs.bottomRight.getAngle(),
		a3 = vecs.topRight.getAngle(),
		a4 = vecs.topLeft.getAngle();

	//to do: create new sprite pseudorandomly
	float mean = (a1 + a2 + a3 + a4) / 4;
	float variance = abs(a1 - mean) + abs(a2 - mean) + abs(a3 - mean) + abs(a4 - mean);

	log(variance);
	if (variance > 1) {
		auto statue = Statue::createWithType(Statue::SPEED);
		statue->setPosition(chunk->getPosition());
		addSprite(statue);

		SQLUtils::insertSprite(statue);
		log("new statue");
	}
}

SpriteManager* SpriteManager::getInstance() {
	static auto instance = new SpriteManager();
	return instance;
}

SerializableSprite* SpriteManager::createSprite(const string& name) {
	auto it = _createFuncs.find(name);
	CCASSERT(it!=_createFuncs.end(), (name + " not found").c_str());

	//to do
    //auto s = dynamic_cast<AttackableSprite*>((*it).second());
	//CCASSERT(s, (name + " failed to cast").c_str());

	auto s = (*it).second();
	addSprite(s);

	return s;
}

void SpriteManager::onYourMove(EventCustom* event) {
	You* you = (static_cast<YourMoveEvent*>(event->getUserData()))->getWho();
	
	static const int COLLISON_LENGTH = 100;
	for (SerializableSprite* sp : _sprites) {
		if ((sp->getPosition() - you->getPosition()).length() > COLLISON_LENGTH) continue;
		if ((sp->getParent() != _layer)) continue;
		auto item = dynamic_cast<Item*>(sp);
		if (item) you->pick(item);
	}
}