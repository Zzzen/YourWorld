#pragma once

#include "Consumable.h"

class Shi : public Consumable {
public:
	static Shi* create();
	static Shi* createWithJson(const Document& doc);

	void beConsumed(AttackableSprite* user) override;
	string getDetails() const override { return R::getString(R::SHI_DETAILS); }

	string getClassName() const override { return "Shi"; }
};