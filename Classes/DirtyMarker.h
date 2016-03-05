#pragma once

#include "SerializableSprite.h"

USING_NS_CC;


/*
	brief description: indicates that the chunk has been created.
*/
class DirtyMarker : public SerializableSprite {
public:
	CREATE_FUNC(DirtyMarker);

	CREATE_WITH_JSON_FUNC(DirtyMarker);

	string getClassName() const override { return "DirtyMarker"; }
};