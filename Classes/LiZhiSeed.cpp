#include "LiZhiSeed.h"
#include "LiZhi.h"

LiZhiSeed* LiZhiSeed::createWithJson(const Document& json)
{
	auto seed = create();
	if (seed&&seed->initWithJson(json)) {
		return seed;
	}
	else {
		CC_SAFE_DELETE(seed);
		return nullptr;
	}
}

Crop * LiZhiSeed::createCrop()
{
	return LiZhi::create();
}

bool LiZhiSeed::init()
{
	if(!Seed::init())	return false;

	setTexture("liZhiSeed.png");
	
	return true;
}
