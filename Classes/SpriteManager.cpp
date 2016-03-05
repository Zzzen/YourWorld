#include "SpriteManager.h"
#include "Chunk.h"
#include "CommonDefinition.h"
#include "SQLUtils.h"
#include "Human.h"
#include "You.h"
#include "Utils.h"
#include "DirtyMarker.h"



using namespace std;
using namespace rapidjson;

const static int SPRITE_ZORDER = 100;

void SpriteManager::onChunkCreated(EventCustom* event) {
	const Chunk* chunk = (static_cast<ChunkDisjoinWorldEvent*>(event->getUserData()))->getWho();
	Point pos = chunk->getPosition();

	auto sprites = SQLUtils::selectSprites(pair<int, int>(pos.x, pos.x + Chunk::SIDE_LENGTH*TILE_SIZE),
		pair<int, int>(pos.y, pos.y + Chunk::SIDE_LENGTH*TILE_SIZE));
	
	//initialize sprites and add thems to the layer.
	
	//whether this chunk has been created
	bool beenCreated = false;
	for (const auto& map : sprites) {
		if (map.at("className") == "DirtyMarker") {
			beenCreated = true;
			continue;
		}

		auto sp = createSprite(map);
		if(sp) _layer->addChild(sp);
	}

	if (!beenCreated) {
		createNewSprites(chunk);
		auto marker = DirtyMarker::create();
		auto position = chunk->getPosition() +
			Vec2(Chunk::SIDE_LENGTH * TILE_SIZE, Chunk::SIDE_LENGTH * TILE_SIZE) / 2.f;
		marker->setPosition(position);
		_layer->addChild(marker);
	}
}

void SpriteManager::onChunkRemoved(EventCustom* event) {
	const Chunk* chunk = (static_cast<ChunkDisjoinWorldEvent*>(event->getUserData()))->getWho();
	auto box = chunk->getBoundingBox();
	for (auto sp: getAllSprites()) {
		if (box.intersectsRect(sp->getBoundingBox())) {
			SQLUtils::addToCache(sp);
			sp->removeFromParent();
		}
	}
}

void SpriteManager::onMobDied(EventCustom * event)
{
	Mob* mob = (static_cast<MobDieEvent*>(event->getUserData()))->getWho();

	auto rand = RandomHelper::random_real(0.f, 1.f);
	Sprite* dropped = nullptr;
	if (rand < 0.2f) {
		dropped = createSprite("Dabaojian");
	}
	else if(rand < 0.4f) {
		dropped = createSprite("Shi");
	}
	else if (rand < 0.7f) {
		dropped = createSprite("Cao");
	}
	else if (rand < 1.f) {
		dropped = createSprite("LiZhiSeed");
	}

	if (dropped) {
		_layer->addChild(dropped, mob->getLocalZOrder());
		dropped->setPosition(mob->getPosition());
		
		//should be caused by physics engine
		if (dynamic_cast<LivingSprite*>(dropped)) {
			auto pos = mob->getPosition();
			Director::getInstance()->getScheduler()->schedule([pos, dropped](float) {dropped->setPosition(pos); }, dropped, 0.f, 1, .0f, false, "set correct position");
		}
	}
	
	//Director::getInstance()->getScheduler()->schedule([cao](float) {log("... cao %s", str(cao->getPosition()).c_str()); }, cao, 0.3f, false, "14851");


	SQLUtils::removeSprite(mob);
	_layer->removeChild(mob);

	if (_vas.find(mob) != _vas.end()) {
		_vas.erase(_vas.find(mob));
	}
}

SerializableSprite* SpriteManager::createSprite(const unordered_map<string, string>& map) {
	const int x = atoi(map.at("x").c_str()),
		      y = atoi(map.at("y").c_str());
	const string className = map.at("className"),
                 properties = map.at("properties");

	
	int64_t rowid = strTo<int64_t>(map.at("rowid"));
	//CC_ASSERT(rowid != 0);

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
	sprite->setRowid(rowid);
	sprite->setLocalZOrder(SPRITE_ZORDER);

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

	log("variance: %4.3f", variance);
	while (variance>1.f)
	{
		variance -= 0.6f;
		float chunkSize = Chunk::SIDE_LENGTH * TILE_SIZE;
		float dx = sin(variance * 123) / 2.f + 0.5f;
		float dy = sin(variance * 321) / 2.f + 0.f;
		auto boulder = createSprite("Boulder");
		boulder->setPosition(chunk->getPosition() + Vec2(dx * chunkSize, dy * chunkSize));

		log("boudler: %s", str(boulder->getPosition()).c_str());                                  

		_layer->addChild(boulder, SPRITE_ZORDER);
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

SerializableSprite* SpriteManager::createSpriteWithJson(const string& className, const Document& doc) {
	auto it = _createFuncsWithJson.find(className);

	if (it != _createFuncsWithJson.end()) {
		return (*it).second(doc);
	}
	else {
		log("%s createWithJson not found.", className.c_str());
		return nullptr;
	}
}

void SpriteManager::onYourMove(EventCustom* event) {
	You* you = (static_cast<YourMoveEvent*>(event->getUserData()))->getWho();
	
	static const int COLLISON_LENGTH = 20;
	for (SerializableSprite* sp : getAllSprites()) {
		if ((sp->getPosition() - you->getPosition()).length() > COLLISON_LENGTH) continue;
		if ((sp->getParent() != _layer)) continue;
		auto item = dynamic_cast<Item*>(sp);
		if (item && item->isPickable()) you->pick(item);
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

void SpriteManager::updateVas(float dt)
{
	_vas.clear();
	static const float LENGTH = 500.f;
	for (auto& sp : getAllSprites()) {
		auto a = dynamic_cast<AttackableSprite*>(sp);
		if (a && (a->getPosition() - You::getInstance()->getPosition()).lengthSquared() < LENGTH * LENGTH) {
			_vas.pushBack(a);
		}
	}
}
