#pragma once

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

struct PhysicsBodyConfig {
	enum Tag {
		MOB_TAG = 100,
		BULLET_TAG = 101,
		YOU_TAG = 102
	};

	Tag tag;
	int categoryBitmask;
	int collisionBitmask;
	int contactTestBitmask;

	static const PhysicsBodyConfig MOB;
	static const PhysicsBodyConfig BULLET;
	static const PhysicsBodyConfig YOU;
};

static const int TILE_SIZE = 16;

static const char* DEFUALT_FONT = "fonts/Marker Felt.ttf";
