#include "SQLUtils.h"

#include "Utils.h"
#include "lib/sqlite3pp/sqlite3pp.h"
#include "CCFileUtils.h"
#include "ChunkManager.h"

#include "sqlite3.h"

#include <vector>

#define RAPIDJSON_HAS_STDSTRING 1
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"

using namespace sqlite3pp;
using namespace rapidjson;
using namespace std;

static const string TABLE_NAME = "sprites";

static database& getDBInstance() {
	static std::string fileName = cocos2d::FileUtils::getInstance()->getWritablePath()+"data.sqlite3";
	static database db(fileName.c_str());
	return db;
}

// rects that have been read from database. they may intersected.
static vector<Rect> cachedRecs;

// <rowid, <field name, value>> 
static unordered_map<int64_t, unordered_map<string, string>> spritesWithId;

// sprites to insert have a invalid rowid, ie 0.
static vector<unordered_map<string, string>> spritesWithoutId;

static Point getPosition(const unordered_map<string, string>& map) {
	int x = strTo<int>(map.at("x").c_str());
	int y = strTo<int>(map.at("y").c_str());
	return Point(x, y);
}

static vector<unordered_map<string, string>> getCachedSprites(const Rect& rect) {
	vector<unordered_map<string, string>> toReturn;
	for (auto it = spritesWithId.begin(); it != spritesWithId.end();) {
		if (rect.containsPoint(getPosition((*it).second))) {
			toReturn.push_back((*it).second);
			it = spritesWithId.erase(it);
		}
		else {
			it++;
		}
	}
	for (auto it = spritesWithoutId.begin(); it != spritesWithoutId.end();) {
		if (rect.containsPoint(getPosition(*it))) {
			toReturn.push_back(*it);
			it = spritesWithoutId.erase(it);
		}
		else {
			it++;
		}
	}

	return toReturn;
}

void SQLUtils::flush() {
	auto& db = getDBInstance();
	auto raw = db.getRawDb();


	sqlite3_stmt* stmt = nullptr;
	string update =
		"UPDATE " + TABLE_NAME + " SET "
		" x = ?, y = ?, properties = ? "
		"WHERE rowid = ?;";
	CCASSERT( sqlite3_prepare_v2(raw, update.c_str(), -1, &stmt, 0) == SQLITE_OK, "unable to prepare update stmt");

	for(auto it = spritesWithId.begin(); it != spritesWithId.end();it++)
	{
		const auto& map = (*it).second;
		sqlite3_bind_int(stmt, 1, strTo<int>(map.at("x")));
		sqlite3_bind_int(stmt, 2, strTo<int>(map.at("y")));
		sqlite3_bind_text(stmt, 3, map.at("properties").c_str(), map.at("properties").size(), SQLITE_TRANSIENT);
		sqlite3_bind_int64(stmt, 4, strTo<int>(map.at("rowid")));

		CCASSERT(sqlite3_step(stmt) == SQLITE_DONE,"update sqlite3_step(stmt) != SQLITE_DONE ");
		sqlite3_reset(stmt);


		CCLOG(" update ok ");
	}

	CCASSERT(sqlite3_finalize(stmt)==SQLITE_OK, "sqlite3_finalize(stmt)!=SQLITE_OK");
	stmt = nullptr;


	string insert =
		"INSERT INTO " + TABLE_NAME +
		"(x, y, className, properties) "
		"VALUES (?,?,?,?);";
	CCASSERT(sqlite3_prepare_v2(raw, insert.c_str(), -1, &stmt, 0) == SQLITE_OK, " unable to prepare insert stmt ");
	for (auto it = spritesWithoutId.begin(); it != spritesWithoutId.end();) {
		const auto& map = *it;

		sqlite3_bind_int(stmt, 1, strTo<int>(map.at("x")));
		sqlite3_bind_int(stmt, 2, strTo<int>(map.at("y")));
		sqlite3_bind_text(stmt, 3, map.at("className").c_str(), map.at("className").size(), SQLITE_TRANSIENT);
		sqlite3_bind_text(stmt, 4, map.at("properties").c_str(), map.at("properties").size(), SQLITE_TRANSIENT);

		CCASSERT(sqlite3_step(stmt) == SQLITE_DONE, "insert sqlite3_step(stmt) != SQLITE_DONE ");
		sqlite3_reset(stmt);


		int64_t rowid = sqlite3_last_insert_rowid(raw);
		CCLOG("last rowid %d", (int)rowid);
		spritesWithId[rowid] = map;

		it = spritesWithoutId.erase(it);
	}

	CCASSERT(sqlite3_finalize(stmt) == SQLITE_OK, "sqlite3_finalize(stmt)!=SQLITE_OK");
	stmt = nullptr;
}

static bool isRectCached(const Rect& rect) {
	for (const auto& r : cachedRecs) {
		if (r.containsPoint(Point(rect.getMidX(), rect.getMidY()))) {
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
	string create = string("CREATE TABLE ") + TABLE_NAME + "("
		"x INT NOT NULL,				"
		"y INT NOT NULL,				"
		"className TEXT NOT NULL,   	"
		"properties TEXT   );			";
	db.execute(create.c_str());
	//log("error msg: %s", db.error_msg());
}

void SQLUtils::addToCache(const SerializableSprite* sprite){
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
	map["rowid"] = to_string(sprite->getRowid());

	if (sprite->getRowid() != 0) {
		spritesWithId[sprite->getRowid()] = map;
	}
	else {
		spritesWithoutId.push_back(map);
	}

	return;
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

	string statement("SELECT x, y, className, properties, rowid"
		" FROM sprites"
		" WHERE x BETWEEN " + to_string(xLow) + " AND  " + to_string(xUp) +
		"   AND y BETWEEN " + to_string(yLow) + " AND  " + to_string(yUp));

	CCLOG("reading from database xLow: %d, xUp: %d, yLow: %d, yUp: %d", xLow, xUp, yLow, yUp);

	query qry(db, statement.c_str());

	for (auto it = qry.begin(); it != qry.end(); ++it) {
		unordered_map<string, string> map;

		for (int j = 0; j < qry.column_count(); ++j) {
			map[qry.column_name(j)] = (*it).get<char const*>(j);
		}

		int64_t rowid = strTo<int64_t>(map.at("rowid"));
		spritesWithId[rowid] = map;
	}

	return getCachedSprites(rect);
}