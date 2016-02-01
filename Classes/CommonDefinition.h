#ifndef __COMMONDEFINITION_H__
#define __COMMONDEFINITION_H__

#include "math\Vec2.h"

USING_NS_CC_MATH;

/**
@struct GradientVectors
@brief contains 4 2d vectors
*/
struct GradientVectors
{
	Vec2 bottomLeft,
		bottomRight,
		topRight,
		topLeft;
	/**
	* Construct an immutable `GradientVectors`.
	*/
	GradientVectors(const Vec2& bottomLeft, const Vec2& bottomRight, const Vec2& topRight, const Vec2& topLeft) :
		bottomLeft(bottomLeft.getNormalized()),
		bottomRight(bottomRight.getNormalized()),
		topRight(topRight.getNormalized()),
		topLeft(topLeft.getNormalized())
	{
	}
};

struct GridCoordinate
{
	int x;
	int y;
};

static const size_t TILE_SIZE = 16;

static const char* DEFUALT_FONT = "fonts/Marker Felt.ttf";

#endif