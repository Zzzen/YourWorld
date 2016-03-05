#include "Rock.h"

Rock * Rock::create()
{
	auto rock = new Rock();
	if (rock->init()) {
		rock->setTexture("rock.png");
		rock->autorelease();
		return rock;
	}
	else
	{
		CC_SAFE_DELETE(rock);
		return nullptr;
	}
}

Rock::Rock()
{
}
