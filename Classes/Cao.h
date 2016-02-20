#pragma once

#include "Crop.h"

USING_NS_CC;

class Cao : public Crop {
public:
	CREATE_FUNC(Cao);

	static Cao* createWithJson(const rapidjson::Document& json);

	bool isPluckable() const override;
	void bePlucked() override;

	void die() override {};

	string getClassName() const { return "Cao"; }
private:
	AnimationConfig getAnimationConfig() const override { return AnimationConfig::cao; }
};