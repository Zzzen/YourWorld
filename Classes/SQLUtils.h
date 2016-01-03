#pragma once

#include "SerializableSprite.h"

using namespace std;
namespace SQLUtils {
	void createTable();
	void insertSprite(const SerializableSprite* sprite);
}