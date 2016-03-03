#include "Mineral.h"
#include "MiningEventData.h"

Mineral::Mineral():
	_miningListener(nullptr)
{
	_miningListener = EventListenerCustom::create(MiningEventData::getName(), 
		[this](EventCustom * eventCustom){
		auto data = static_cast<MiningEventData*>(eventCustom->getUserData());
		assert(data);
		if (data) {
			onMined(data);
		}
	});
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_miningListener, this);
}

void Mineral::enableMiningListener()
{
	_miningListener->setEnabled(true);
}

void Mineral::disableMiningListener()
{
	_miningListener->setEnabled(false);
}

