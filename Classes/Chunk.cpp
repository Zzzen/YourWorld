#include "Chunk.h"

const std::string ChunkJoinWorldEvent::EVENT_CHUNK_JOIN_WORLD = "event_of_chunk_join_world";
const std::string ChunkDisjoinWorldEvent::EVENT_CHUNK_DISJOIN_WORLD = "event_of_chunk_disjoin_world";
enum TerrainType {
	LAWN = 1,
	POND = 2,
	DESERT = 3,
	JUNGLE = 4
};

bool Chunk::initTiles(){
	auto map = cocos2d::experimental::TMXTiledMap::create("tile.tmx");
	assert(map);
	addChild(map);
	auto layer = map->getLayer("layer");
	assert(layer);

	for (size_t x = 0; x < SIDE_LENGTH; x++){
		for (size_t y = 0; y < SIDE_LENGTH; y++){
			Vec2 distance_bl(x, y);
			Vec2 distance_br(SIDE_LENGTH - x, y);
			Vec2 distance_tr(SIDE_LENGTH - x, SIDE_LENGTH - y);
			Vec2 distance_tl(x, SIDE_LENGTH - y);

			float bl = distance_bl.dot(_gradientVectors.bottomLeft);
			float br = distance_br.dot(_gradientVectors.bottomRight);
			float tr = distance_tr.dot(_gradientVectors.topRight);
			float tl = distance_tl.dot(_gradientVectors.topLeft);

			//linear interpolation
			float bottom = lerp(bl, br, x / (float)SIDE_LENGTH);
			float top = lerp(tl, tr, x / (float)SIDE_LENGTH);
			float bt = lerp(bottom, top, y / (float)SIDE_LENGTH);

			TerrainType type = TerrainType::LAWN;
			if (bt < 0){
				type = TerrainType::POND;
			}
			else if (bt < 9){
				type = TerrainType::JUNGLE;
			}
			else if (bt < 15){
				type = TerrainType::LAWN;
			}
			else{
				type = TerrainType::DESERT;
			}

			layer->setTileGID(type, Vec2(x, SIDE_LENGTH -1 - y));
		}
	}
	return true;
}

Chunk* Chunk::createWithGradientVectors(const GradientVectors& vectors) {
	auto chunk = new(std::nothrow) Chunk(vectors);
	if (chunk && chunk->init())
	{
		chunk->autorelease();
		return chunk;
	}
	else
	{
		CC_SAFE_DELETE(chunk);
		return nullptr;
	}
}

bool Chunk::init() {
	if (!Node::init()) {
		return false;
	}
	return initTiles();
}

Chunk::Chunk(const GradientVectors& vectors):
  _gradientVectors(vectors)
{
	setContentSize(Size(SIDE_LENGTH*TILE_SIZE, SIDE_LENGTH*TILE_SIZE));
	setAnchorPoint(Vec2(0, 0));

}

Chunk::~Chunk()
{
	CCLOG("Chunk removed");
}


/*************************************************************************/
ChunkEvent::ChunkEvent(Chunk * chunk)
	:_chunk(chunk)
{
	_chunk->retain();
}

Chunk * ChunkEvent::getChunk() const
{
	return _chunk;
}

ChunkEvent::~ChunkEvent() {
	_chunk->release();
}

ChunkJoinWorldEvent * ChunkJoinWorldEvent::createWithChunk(Chunk* chunk)
{
	auto event = new (std::nothrow) ChunkJoinWorldEvent(chunk);
	assert(event);
	event->autorelease();

	return event;
}

ChunkDisjoinWorldEvent * ChunkDisjoinWorldEvent::createWithChunk(Chunk* chunk)
{
	auto event = new (std::nothrow) ChunkDisjoinWorldEvent(chunk);
	assert(event);
	event->autorelease();

	return event;
}