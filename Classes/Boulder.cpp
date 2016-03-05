#include "Boulder.h"

#include "Rock.h"

Boulder * Boulder::create()
{
	auto boulder = new Boulder();
	if (boulder->init()) {
		boulder->setTexture("boulder.png");
		boulder->autorelease();
		return boulder;
	}
	else {
		CC_SAFE_DELETE(boulder);
		return nullptr;
	}
}

Document Boulder::toJson() const
{
	auto json = Mineral::toJson();
	json.AddMember("progress", rapidjson::Value(getProgress()), json.GetAllocator());
	return json;
}

bool Boulder::initWithJson(const Document & json)
{
	if (!Mineral::initWithJson(json)) {
		return false;
	}

	assert(json.HasMember("progress") && json["progress"].IsInt());
	setProgress(json["progress"].GetInt());

	return true;
}

void Boulder::onMined(const MiningEventData& data)
{
	setProgress(getProgress() + data.getDamage());
}

void Boulder::setProgress(int progress)
{
	_progress = progress;
	if (_progress > 100) {
		//... add Rocks 

		int numberOfRocks = RandomHelper::random_int(1, 3);
		for (int i = 0; i < numberOfRocks; i++) {
			auto rock = Rock::create();
			rock->setPosition(getPosition());

			getParent()->addChild(rock, getLocalZOrder());

			auto delta = Vec2::forAngle(RandomHelper::random_real(0.f, 2 * 3.1415f)) * 30;
			auto move = MoveBy::create(1.f, delta);
			rock->runAction(move);
		}

		removeFromParent();
	}
}

Boulder::Boulder():
	_progress(0)
{
}
