#ifndef __CHUNK_H__
#define __CHUNK_H__

#include "CommonDefinition.h"
#include "cocos2d.h"
#include "SpriteEventData.h"

class Chunk : public Node
{
public:
	static Chunk* createWithGradientVectors(const GradientVectors& vectors);

	~Chunk() override;

	bool initTiles(const GradientVectors& vectors);

	//there are 128 tiles along one side.
	const static int SIDE_LENGTH = 128;

	GradientVectors getGradientVectors() const { return _gradientVectors; }

	//@param pos in cocos2d-x coordinate.
	void setTileGID(int gid, const Vec2& pos);
	//see above
	int getTileGID(const Vec2& pos) const;
protected:
	Chunk();

	bool init(const GradientVectors& vectors);
	/*
	* @param weight: the weight of y, 0<= weight <=1.
	*/
	inline float lerp(float x, float y, float weight) const {
		return (1 - weight)*x + weight*y;
	}

	inline Vec2 toTmxCoordinate(const Vec2& pos)const;
	
	GradientVectors _gradientVectors;
	cocos2d::experimental::TMXTiledMap* _map;
};

/*
	@brief holds a reference to Chunk, retains and releases the ownership automatically.
*/

class ChunkJoinWorldEvent : public SpriteEventData<Chunk> {
public:
	static ChunkJoinWorldEvent* createWithWho(Chunk* const sprite) {
		auto event = new ChunkJoinWorldEvent();
		event->initWithWho(sprite);

		event->autorelease();
		return event;
	}

	static const std::string getName() {
		return "chunk_join_world_event";
	}
};

class ChunkDisjoinWorldEvent: public SpriteEventData<Chunk>
{
public:
	static ChunkDisjoinWorldEvent* createWithWho(Chunk* const sprite) {
		auto event = new ChunkDisjoinWorldEvent();
		event->initWithWho(sprite);

		event->autorelease();
		return event;
	}

	static const std::string getName() {
		return "chunk_disjoin_world_event";
	}
};




#endif