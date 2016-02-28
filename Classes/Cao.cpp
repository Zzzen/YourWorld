#include "Cao.h"

Cao * Cao::createWithJson(const rapidjson::Document & json)
{
	auto cao = create();

	if (cao && cao->initWithJson(json)) {
		return cao;
	}
	else {
		CC_SAFE_DELETE(cao);
		return nullptr;
	}
}

bool Cao::isPluckable() const
{
	return true;
}

void Cao::bePlucked()
{
}
