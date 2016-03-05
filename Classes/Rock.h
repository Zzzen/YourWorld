#pragma once

#include "Item.h"

USING_NS_CC;

class Rock : public Item {
public:
	static Rock* create();
	CREATE_WITH_JSON_FUNC(Rock);

	Rock();

	string getClassName() const override { return "Rock"; }
	
	Item::Type getItemType() const override { return Item::Type::OTHERS; }

	string getDetails() const override { return "to do"; }

};