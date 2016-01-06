#ifndef __CHUNK_H__
#define __CHUNK_H__

#include "CommonDefinition.h"
#include "cocos2d.h"
#include "SpriteEvent.h"

class Chunk : public Node
{
public:
	static Chunk* createWithGradientVectors(const GradientVectors& vectors);

	~Chunk() override;

	//there are 64 tiles along one side.
	const static size_t SIDE_LENGTH = 128;

	GradientVectors getGradientVectors() const { return _gradientVectors; }
protected:
	Chunk(const GradientVectors& vectors);

	bool init() override;
	bool initTiles();
	/*
	* @param weight: the weight of y, 0<= weight <=1.
	*/
	inline float lerp(float x, float y, float weight) const {
		return (1 - weight)*x + weight*y;
	}
	
	const GradientVectors _gradientVectors;
};

/*
	@brief holds a reference to Chunk, retains and releases the ownership automatically.
*/

class ChunkJoinWorldEvent : public SpriteEvent<Chunk> {
public:
	static const std::string getName() {
		return "chunk_join_world_event";
	}
};

class ChunkDisjoinWorldEvent: public SpriteEvent<Chunk>
{
public:
	static const std::string getName() {
		return "chunk_disjoin_world_event";
	}
};




#endif