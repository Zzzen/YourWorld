#ifndef __CHUNK_H__
#define __CHUNK_H__

#include "CommonDefinition.h"
#include "cocos2d.h"
#include "CommonDefinition.h"


//Chunk will be removed from its parent in the dtor.
class Chunk : public Node
{
public:
	Chunk(const GradientVectors& vectors);

	~Chunk() override;

	//there is 64 tiles along one side.
	const static size_t SIDE_LENGTH = 64;

protected:
	bool initTiles();
	/*
	* @param weight: the weight of y, 0<= weight <=1.
	*/
	inline float lerp(float x, float y, float weight){
		return (1 - weight)*x + weight*y;
	}
	
	const GradientVectors _gradientVectors;
};

#endif