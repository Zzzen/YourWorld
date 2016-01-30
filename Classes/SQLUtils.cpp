#include "SQLUtils.h"

#include "Utils.h"
#include "lib/sqlite3pp/sqlite3pp.h"
#include "CCFileUtils.h"
#include "ChunkManager.h"

#include <vector>

#define RAPIDJSON_HAS_STDSTRING 1
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"

using namespace sqlite3pp;
using namespace rapidjson;
using namespace std;

static database& getDBInstance() {
	static std::string fileName = cocos2d::FileUtils::getInstance()->getWritablePath()+"data.sqlite3";
	static database db(fileName.c_str());
	return db;
}

// rects that have been read from database. they may intersected.
static vector<Rect> cachedRecs;

static vector<unordered_map<string, string>> cachedSprites;

static vector<unordered_map<string, string>> spritesToInsert;

static Point getPosition(const unordered_map<string, string>& map) {
	int x = atoi(map.at("x").c_str());
	int y = atoi(map.at("y").c_str());
	return Point(x, y);
}

static bool isSpritesCached(const unordered_map<string, string>& sp) {
	for (const auto& map : cachedSprites) {
		if (getPosition(map).distanceSquared(getPosition(sp)) < 0.01f) {
			return true;
		}
	}
	return false;
}

static vector<unordered_map<string, string>> getCachedSprites(const Rect& rect) {
	vector<unordered_map<string, string>> toReturn;
	for (auto it = cachedSprites.begin(); it != cachedSprites.end();) {
		if (rect.containsPoint(getPosition(*it))) {
			toReturn.push_back(*it);
			it = cachedSprites.erase(it);
		}
		else {
			it++;
		}
	}
	return toReturn;
}

static bool isRectCached(const Rect& rect) {
	for (const auto& r : cachedRecs) {
		if (r.containsPoint(Point(rect.getMidX(), rect.getMinY()))) {
			return true;
		}
	}
	return false;
}

static string getJsonStringFromSprite(const SerializableSprite* sprite){
	StringBuffer sb;
	Writer<StringBuffer> writer(sb);

	const auto& json = sprite->toJson();
	
	json.Accept(writer);

	return sb.GetString();
}

void SQLUtils::createTable() {
	auto& db = getDBInstance();
	db.execute("CREATE TABLE sprites(		"
			   "x INT NOT NULL,				"
			   "y INT NOT NULL,				"
			   "className TEXT NOT NULL,	"
			   "properties TEXT);			");
	//log("error msg: %s", db.error_msg());
}

void SQLUtils::insertSprite(const SerializableSprite* sprite){
	auto& pos = sprite->getPosition();
	const int x = static_cast<int>(pos.x),
		y = static_cast<int>(pos.y);
	auto& className = sprite->getClassName();

	const auto& properties = getJsonStringFromSprite(sprite);

    //*******************************
	//add to cache and return.
	unordered_map<string, string> map;
	map["x"] = to_string(x);
	map["y"] = to_string(y);
	map["className"] = className;
	map["properties"] = properties;

	spritesToInsert.push_back(map);

	return;
	//********************************

	auto& db = getDBInstance();

	sqlite3pp::command cmd(db,
		"INSERT INTO sprites (x, y, className, properties)"
		"             VALUES (?,?,?,?);");
	//cmd.binder() << x << y << className<< properties;
	cmd.bind(1, x);
	cmd.bind(2, y);
	cmd.bind(3, className, copy_semantic::copy);
	cmd.bind(4, properties, copy_semantic::copy);
	cmd.execute();

	//log("insert msg: %s", db.error_msg());
}

vector<unordered_map<string, string>> SQLUtils::selectSprites(const pair<int, int>& xRange, const pair<int, int>& yRange) {
	//if it has been cached.
	Rect rect(xRange.first, xRange.second, xRange.second - xRange.first, yRange.second);
	if (isRectCached(rect)) {
		return getCachedSprites(rect);
	}

	int xLow = xRange.first - 10 * ChunkManager::CHUNK_WIDTH;
	int xUp = xRange.second + 10 * ChunkManager::CHUNK_WIDTH;
	int yLow = yRange.first - 10 * ChunkManager::CHUNK_HEIGHT;
	int yUp = yRange.second + 10 * ChunkManager::CHUNK_HEIGHT;

	cachedRecs.push_back(Rect(xLow, yLow, xUp - xLow, yUp - yLow));

	auto& db = getDBInstance();

	string statement("SELECT x, y, className, properties"
		" FROM sprites"
		" WHERE x> " + to_string(xLow) + " AND x< " + to_string(xUp) +
		"   AND y> " + to_string(yLow) + " AND y< " + to_string(yUp));

	CCLOG("reading from database xLow: %d, xUp: %d, yLow: %d, yUp: %d", xLow, xUp, yLow, yUp);

	query qry(db, statement.c_str());

	for (auto it = qry.begin(); it != qry.end(); ++it) {
		unordered_map<string, string> map;

		for (int j = 0; j < qry.column_count(); ++j) {
			map[qry.column_name(j)] = (*it).get<char const*>(j);
		}

		if (!isSpritesCached(map)) {
			cachedSprites.push_back(map);
		}
	}

	return getCachedSprites(rect);
}