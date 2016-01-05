#pragma once

#include "SerializableSprite.h"

using namespace std;
namespace SQLUtils {
	void createTable();
	void insertSprite(const SerializableSprite* sprite);
	vector<unordered_map<string, string>> selectSprites(const pair<int, int>& xRange, const pair<int, int>& yRange);
}