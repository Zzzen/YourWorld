#pragma once


#include "SerializableSprite.h"

class Item : public SerializableSprite {
public:
	enum Type {
		EQUIPMENTS = 1,
		CONSUMABLES = 2,
		OTHERS = 3
	};

	virtual Type getItemType() const = 0;

	//play animation
	void onEnter() override;
	void onExit() override;

};