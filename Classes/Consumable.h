#pragma once

#include "Item.h"

class AttackableSprite;
class Consumable : public Item {
public:
	Type getItemType()const override { return CONSUMABLES; }

	virtual void beConsumed(AttackableSprite* ) = 0;
};