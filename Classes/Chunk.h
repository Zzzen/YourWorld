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
	const static size_t SIDE_LENGTH = 64;

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

#endif