#pragma once

#include "SerializableSprite.h"

using namespace std;
namespace SQLUtils {

	void createTable();
	
	//insert into cache.
	void addToCache(const SerializableSprite* sprite);
	
	//flush to database.
	void flush();
	
	vector<unordered_map<string, string>> selectSprites(const pair<int, int>& xRange, const pair<int, int>& yRange);
}