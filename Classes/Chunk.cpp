#include "Chunk.h"

bool Chunk::initTiles(){
	auto map = cocos2d::experimental::TMXTiledMap::create("tile.tmx");
	addChild(map);
	auto layer = map->getLayer("layer");

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

			int gid = 1;
			if (bt < -30){
				gid = 1;
			}
			else if (bt < 0){
				gid = 2;
			}
			else if (bt < 30){
				gid = 3;
			}
			else{
				gid = 4;
			}

			layer->setTileGID(gid, Vec2(x, SIDE_LENGTH -1 - y));
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
		delete chunk;
		chunk = nullptr;
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