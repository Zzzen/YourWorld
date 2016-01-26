#pragma once

#include "Consumable.h"

class Shi : public Consumable {
public:
	static Shi* create();

	void beConsumed(AttackableSprite* user) override;
	string getDetails() const override { return R::getString(R::SHI_DETAILS); }

	const string getClassName() const override { return "Shi"; }
};