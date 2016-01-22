#pragma once


#include "SerializableSprite.h"

class Item : public SerializableSprite {
public:
	enum Type {

	};

	//play animation
	void onEnter() override;
	void onExit() override;
};