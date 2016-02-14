#pragma once

#include "SerializableSprite.h"

using namespace std;
namespace SQLUtils {
	void setDatabaseFileName(const string& fileName);

	void createTable();
	
	//insert into cache.
	void addToCache(const SerializableSprite* sprite);
	
	//flush to database and return updated rowid.
	unordered_map<int64_t, int64_t>& flush();
	
	vector<unordered_map<string, string>> selectSprites(const pair<int, int>& xRange, const pair<int, int>& yRange);

	unordered_map<string, string> selectYou();
}