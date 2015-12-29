#ifndef __CHUNK_H__
#define __CHUNK_H__

#include "CommonDefinition.h"
#include "cocos2d.h"
#include "CommonDefinition.h"



class Chunk : public Node
{
public:
	static Chunk* createWithGradientVectors(const GradientVectors& vectors);

	~Chunk() override;

	//there are 64 tiles along one side.
	const static size_t SIDE_LENGTH = 128;

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
class ChunkEvent : public Ref {
public:
	Chunk* getChunk() const;
	~ChunkEvent() override;

protected:
	ChunkEvent(Chunk* chunk);
	Chunk* _chunk;
};

class ChunkJoinWorldEvent : public ChunkEvent {
public:
	static ChunkJoinWorldEvent* createWithChunk(Chunk* chunk);
	static const std::string EVENT_CHUNK_JOIN_WORLD;

protected:
	ChunkJoinWorldEvent(Chunk* chunk) : ChunkEvent(chunk) {}
};

class ChunkDisjoinWorldEvent: public ChunkEvent
{
public:
	static ChunkDisjoinWorldEvent* createWithChunk(Chunk* chunk);
	static const std::string EVENT_CHUNK_DISJOIN_WORLD;

protected:
	ChunkDisjoinWorldEvent(Chunk* chunk) : ChunkEvent(chunk) {}
};




#endif