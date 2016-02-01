#include "ChunkManager.h"
#include "PseudorandomNumber.h"
#include "Utils.h"

#include "math\CCMath.h"
#include "cocos2d.h"
#include <vector>

USING_NS_CC;


const float ChunkManager::CHUNK_WIDTH = Chunk::SIDE_LENGTH * TILE_SIZE;
const float ChunkManager::CHUNK_HEIGHT = CHUNK_WIDTH;

const static int MAX_CACHED_THUNK = 3;

ChunkManager::~ChunkManager(){
}

ChunkManager::ChunkManager():
	_parent(nullptr)
{
	for (int i = 0; i < 16; i++) {
		auto chunk = Chunk::createWithGradientVectors(GradientVectors(Vec2::ZERO, Vec2::ZERO, Vec2::ZERO, Vec2::ZERO));
		_chunks.pushBack(chunk);
	}
}

void ChunkManager::updateChunks(const Point& point, bool visible /* = true */ ){
	if (!visible){
		return;
	}

	hideDistantChunk(point);

	if (contains(point)) return;

	newChunk(point);
}

void ChunkManager::hideDistantChunk(const Point& pos) {
	auto onScene = getChunksOnScene();
	for (auto it = onScene.begin(); it != onScene.end(); it++) {
		if (((*it)->getPosition() - pos).length() > Chunk::SIDE_LENGTH*TILE_SIZE * MAX_CACHED_THUNK) {
			auto event = ChunkDisjoinWorldEvent::createWithWho(*it);
			Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(ChunkDisjoinWorldEvent::getName(),
				event);

			CCLOG("hide Chunk at %s ", str((*it)->getPosition()).c_str());
			(*it)->removeFromParent();
			//it = _chunks.erase(it);
		}
	}
}

Chunk* ChunkManager::newChunk(const Point& interior){
	//find the grid coordinate
	auto bottomLeft = convertPointToGrid(interior);
	auto br = GridCoordinate{ bottomLeft.x + 1, bottomLeft.y };
	auto tr = GridCoordinate{ bottomLeft.x + 1, bottomLeft.y +1 };
	auto tl = GridCoordinate{ bottomLeft.x, bottomLeft.y + 1 };
	//get corresponding indexs.
	auto i1 = convertCoorToIndex(bottomLeft);
	auto i2 = convertCoorToIndex(br);
	auto i3 = convertCoorToIndex(tr);
	auto i4 = convertCoorToIndex(tl);

	//get 4 p random angles.
	auto rands = PseudorandomNumber::getInstance();
	float a1 = rands->getNumber(i1);
	float a2 = rands->getNumber(i2);
	float a3 = rands->getNumber(i3);
	float a4 = rands->getNumber(i4);

//	log("bl: %f, br: %f, tr %f, tl %f \n", a1, a2, a3, a4);

	Vec2 v1 = rotatedVector(a1),
		v2 = rotatedVector(a2),
		v3 = rotatedVector(a3),
		v4 = rotatedVector(a4);

	GradientVectors vecs{ v1, v2, v3, v4 };

	//new!
	Chunk* chunk = nullptr;
	for (auto c : getCachedChunks()) {
		if (!c->getParent()) {
			chunk = c;
			break;
		}
	}

	if (chunk) {
		chunk->initTiles(vecs);
		CCLOG("reuse Chunk at %f, %f", bottomLeft.x*CHUNK_WIDTH, bottomLeft.y*CHUNK_HEIGHT);
	}
	else {
		chunk = Chunk::createWithGradientVectors(vecs);
		CCLOG("create chunk at %f, %f", bottomLeft.x*CHUNK_WIDTH, bottomLeft.y*CHUNK_HEIGHT);
		assert(chunk);
	}
	chunk->setPosition(bottomLeft.x*CHUNK_WIDTH, bottomLeft.y*CHUNK_HEIGHT);

	auto event = ChunkJoinWorldEvent::createWithWho(chunk);
	assert(event);
	Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(ChunkJoinWorldEvent::getName(),
		event);
	
	_parent->addChild(chunk);
	if (_chunks.find(chunk) == _chunks.end()) {
		_chunks.pushBack(chunk);
	}

	return chunk;
}

ChunkManager* ChunkManager::getInstance()
{
	static auto instance = new ChunkManager();
	return instance;
}

inline Vec2 ChunkManager::rotatedVector(float angle){
	Vec2 unit(1, 0);
	unit.rotate(Vec2::ZERO, angle);
	return unit;
}

bool ChunkManager::contains(const Point& point) {
	for (auto chunk : getChunksOnScene()){
	//	Rect boundary(chunk->getPosition(), chunk->getContentSize());
		if (chunk->getBoundingBox().containsPoint(point)){
			return true;
		}
	}
	return false;
}

GridCoordinate ChunkManager::convertPointToGrid(const Point& point){
	int x = static_cast<int> (floor(point.x / CHUNK_WIDTH));
	int y = static_cast<int> (floor(point.y / CHUNK_HEIGHT));

	return GridCoordinate{ x, y };
}
int ChunkManager::convertCoorToIndex(const GridCoordinate& grid){
	int index = 0;
	int x = grid.x;
	int y = grid.y;
//	CCLOG("convertCoorToIndex: (%d, %d)", x, y);
	while (true)
	{
		if (x == 0 && y == 0){
			index += 1;
			break;
		}
		else if (x > 0 && x == -y){
			index += round(pow(2 * x + 1, 2));
			break;
		}
		else if (x>0 && x>= abs(y))
		{
			index += round(pow(2 * x - 1, 2)) + x + y;
			break;
		}
		else if (y > 0 && y >= abs(x)){
			index += y - x;
			x = y;
			continue;
		}
		else if (x < 0 && abs(x) >= y){
			index += -(x + y);
			y = -x;
			continue;
		}
		else {
			index += x - y;
			x = y;
		}
	}

//	log("index: %d", index);
	return index;
}

Vector<Chunk*> ChunkManager::getChunksOnScene()
{
	Vector<Chunk*> onScene;
	
	for (auto c : _chunks) {
		if (c->getParent()) {
			onScene.pushBack(c);
		}
	}

	return onScene;
}

Vector<Chunk*> ChunkManager::getCachedChunks()
{
	Vector<Chunk*> cached;
	for (auto c : _chunks) {
		if (!c->getParent()) {
			cached.pushBack(c);
		}
	}
	return cached;
}
