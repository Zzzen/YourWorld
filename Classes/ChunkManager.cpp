#include "ChunkManager.h"
#include "PseudorandomNumber.h"

#include "math\CCMath.h"
#include "cocos2d.h"
#include <vector>

USING_NS_CC;


const static float DX = Chunk::SIDE_LENGTH * TILE_SIZE;
const static float DY = DX;

const static float PI = 3.1415926f;

ChunkManager::ChunkManager(Node* parent)
 : _parent(parent)
{

}

ChunkManager::~ChunkManager(){
}

void ChunkManager::updateChunks(const Point& point, bool visible){
	if (!visible){
		return;
	}

	discardInvisibleChunk(point);

	if (contains(point)) return;

	newChunk(point);
}

void ChunkManager::discardInvisibleChunk(const Point& pos) {
	std::vector<Chunk*> invisible;
	for (auto chunk : _chunks) {
		if ((chunk->getPosition() - pos).length() > Chunk::SIDE_LENGTH*TILE_SIZE * 10.0f) {
			invisible.push_back(chunk);
		}
	}
	for (auto chunk : invisible) {
		auto event = ChunkDisjoinWorldEvent::createWithChunk(chunk);
		Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(ChunkDisjoinWorldEvent::EVENT_CHUNK_DISJOIN_WORLD,
			event);

		chunk->removeFromParent();
		_chunks.eraseObject(chunk);
	}
}

Chunk* ChunkManager::newChunk(const Point& interior){
	//find the grid coordinate
	auto bottomLeft = convertPointToGrid(interior);
	auto br = GridCoordinate{ bottomLeft.x + 1, bottomLeft.y };
	auto tr = GridCoordinate{ bottomLeft.x + 1, bottomLeft.y +1 };
	auto tf = GridCoordinate{ bottomLeft.x, bottomLeft.y + 1 };
	log("new Chunk at ( %d, %d ) gridcoordinate.", bottomLeft.x, bottomLeft.y);
	//get corresponding indexs.
	auto i1 = convertCoorToIndex(bottomLeft);
	auto i2 = convertCoorToIndex(br);
	auto i3 = convertCoorToIndex(tr);
	auto i4 = convertCoorToIndex(tf);
	//get 4 p random angles.
	auto rands = PseudorandomNumber::getInstance();
	float a1 = rands->getNumber(i1);
	a1 = fmod(a1, PI);
	float a2 = rands->getNumber(i2);
	a2 = fmod(a2, PI);
	float a3 = rands->getNumber(i3);
	a3 = fmod(a3, PI);
	float a4 = rands->getNumber(i4);
	a4 = fmod(a4, PI);

	log("bl: %f, br: %f, tl %f, tr %f", a1, a2, a3, a4);

	Vec2 v1 = rotatedVector(a1),
		v2 = rotatedVector(a2),
		v3 = rotatedVector(a3),
		v4 = rotatedVector(a4);

	GradientVectors vecs{ v1, v2, v3, v4 };

	//new!
	auto chunk = Chunk::createWithGradientVectors(vecs);
	chunk->setPosition(bottomLeft.x*DX, bottomLeft.y*DY);

	auto event = ChunkJoinWorldEvent::createWithChunk(chunk);
	Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(ChunkJoinWorldEvent::EVENT_CHUNK_JOIN_WORLD,
		event);
	
	_parent->addChild(chunk);
	_chunks.pushBack(chunk);

	return chunk;
}

inline Vec2 ChunkManager::rotatedVector(float angle){
	Vec2 unit(1, 0);
	unit.rotate(Vec2::ZERO, angle);
	return unit;
}

bool ChunkManager::contains(const Point& point) const {
	for (auto chunk : _chunks){
	//	Rect boundary(chunk->getPosition(), chunk->getContentSize());
		if (chunk->getBoundingBox().containsPoint(point)){
			return true;
		}
	}
	return false;
}

GridCoordinate ChunkManager::convertPointToGrid(const Point& point){
	int x = static_cast<int> (floor(point.x / DX));
	int y = static_cast<int> (floor(point.y / DY));

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