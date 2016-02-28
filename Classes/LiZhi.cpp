#include "LiZhi.h"
#include "MyTime.h"

LiZhi * LiZhi::createWithJson(const Document & json)
{
	auto liZhi = create();
	assert(liZhi);
	if (liZhi->initWithJson(json)) {
		return liZhi;
	}
	else {
		CC_SAFE_DELETE(liZhi);
		return nullptr;
	}
}

void LiZhi::updateCustom(float dt)
{
	if (isPluckable()) {
		_stateAction->gotoFrameAndPlay(5, true);
	}
}

bool LiZhi::isPluckable() const
{
	auto elapsedSecs = MyTime::getInstance()->toRealSec() - getTimeOfBirth() / 1000;
	return elapsedSecs > 30;
}

void LiZhi::bePlucked()
{
}

