#pragma once

#include "SerializableSprite.h"
#include "MiningEventData.h"

USING_NS_CC;

class Mineral : public SerializableSprite {
public:
	virtual void onMined(MiningEventData * data) = 0;

	//_miningLisener is enabled by default.
	virtual void enableMiningListener();
	virtual void disableMiningListener();
	
	Mineral();

private:
	EventListenerCustom* _miningListener;
};