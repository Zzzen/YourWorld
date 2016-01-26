#pragma once

#include "Item.h"
#include "AttackableSprite.h"

class Equipment : public Item {
public:
	AttackableSprite::EquipmentType _equipmentType;
	virtual void attach(AttackableSprite* owner) = 0;
	virtual void detach(AttackableSprite* owner) = 0;

	Type getItemType()const override { return EQUIPMENTS; }
};