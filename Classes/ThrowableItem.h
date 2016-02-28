#pragma once

#include "Item.h"

USING_NS_CC;

class AttackableSprite;
class ThrowableItem : public Item {
public:
	virtual void beThrowed(AttackableSprite* owner, const Vec2& dst) = 0;

	Type getItemType()const override { return Type::THROWABLE; }
};