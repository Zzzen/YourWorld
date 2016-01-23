#include "SpriteManager.h"
#include "Chunk.h"
#include "CommonDefinition.h"
#include "SQLUtils.h"
#include "Human.h"
#include "You.h"




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
		auto sp = createSprite(map);
		if(sp) _layer->addChild(sp);
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
	for (auto sp: getAllSprites()) {
		if (box.intersectsRect(sp->getBoundingBox())) {
			SQLUtils::insertSprite(sp);
			sp->removeFromParent();
		}
	}
}

void SpriteManager::onMobDied(EventCustom * event)
{
	Mob* mob = (static_cast<MobDieEvent*>(event->getUserData()))->getWho();
	_layer->removeChild(mob);
	auto dabaojian = createSprite("Dabaojian");
	_layer->addChild(dabaojian);
	dabaojian->setPosition(mob->getPosition());
}

SerializableSprite* SpriteManager::createSprite(const unordered_map<string, string>& map) {
	const int x = stoi(map.at("x")),
		      y = stoi(map.at("y"));
	const string className = map.at("className"),
                 properties = map.at("properties");

	Document json;
	json.Parse(properties.c_str());

	auto it = _createFuncsWithJson.find(className);

	if (it == _createFuncsWithJson.end()) {
		log("unable to instantiate sprite. class name: %s, properties: %s",
			className.c_str(), properties.c_str());
		return nullptr;
	}

	auto sprite = (*it).second(json);
	sprite->setPosition(x, y);

	log("SpriteManager::createSprite: x: %d, y: %d, class: %s, properties: %s",
		x, y, className.c_str(), properties.c_str());

	return sprite;
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
		_layer->addChild(statue, SPRITE_ZORDER);

		SQLUtils::insertSprite(statue);
		log("new statue");
	}
}

SpriteManager* SpriteManager::getInstance() {
	static auto instance = new SpriteManager();
	return instance;
}

SerializableSprite* SpriteManager::createSprite(const string& className) {
	auto it = _createFuncs.find(className);
	CCASSERT(it!=_createFuncs.end(), (className + " not found").c_str());

	//to do
    //auto s = dynamic_cast<AttackableSprite*>((*it).second());
	//CCASSERT(s, (className + " failed to cast").c_str());

	return (*it).second();
}

void SpriteManager::onYourMove(EventCustom* event) {
	You* you = (static_cast<YourMoveEvent*>(event->getUserData()))->getWho();
	
	static const int COLLISON_LENGTH = 20;
	for (SerializableSprite* sp : getAllSprites()) {
		if ((sp->getPosition() - you->getPosition()).length() > COLLISON_LENGTH) continue;
		if ((sp->getParent() != _layer)) continue;
		auto item = dynamic_cast<Item*>(sp);
		if (item) you->pick(item);
	}
}

Vector<SerializableSprite*> SpriteManager::getAllSprites() {
	Vector<SerializableSprite*> sprites;
	for (auto child : _layer->getChildren()) {
		auto s = dynamic_cast<SerializableSprite*>(child);
		if (s) sprites.pushBack(s);
	}
	return sprites;
}