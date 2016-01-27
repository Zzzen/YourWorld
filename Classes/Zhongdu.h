#pragma once

#include "StatusEffect.h"

#include "cocos2d.h"

USING_NS_CC;

class Zhongdu : public StatusEffect {
public:
	CREATE_FUNC(Zhongdu);

	std::string getClassName() const override { return "Zhongdu"; }
protected:
	void begin() override;
	void end() override;

	void updateCustom(float dt) override;

private:
	float getMaxDuration() const override { return 5.0f; }

};