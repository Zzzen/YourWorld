#pragma once

#include "Crop.h"

USING_NS_CC;

class LiZhi : public Crop {
public:
	CREATE_FUNC(LiZhi);

	static LiZhi* createWithJson(const Document& json);

	void updateCustom(float dt) override;

	bool isPluckable() const override;
	void bePlucked() override;

	void die() override {};

	string getClassName() const override { return "LiZhi"; }

private:
	AnimationConfig getAnimationConfig() const override { return AnimationConfig::liZhi; }
};