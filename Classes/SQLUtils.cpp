#include "SQLUtils.h"

#include "lib/sqlite3pp/sqlite3pp.h"
#include "CCFileUtils.h"

#define RAPIDJSON_HAS_STDSTRING 1
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"

using namespace sqlite3pp;
using namespace rapidjson;

static database& getDBInstance() {
	static std::string fileName = cocos2d::FileUtils::getInstance()->getWritablePath()+"data.sqlite3";
	static database db(fileName.c_str());
	return db;
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
	auto& db = getDBInstance();

	string statement("SELECT x, y, className, properties"
		" FROM sprites"
		" WHERE x> " + to_string(xRange.first) + " AND x< " + to_string(xRange.second) +
		"   AND y> " + to_string(yRange.first) + " AND y< " + to_string(yRange.second));

	query qry(db, statement.c_str());

	vector<unordered_map<string, string>> vec;

	for (sqlite3pp::query::iterator i = qry.begin(); i != qry.end(); ++i) {
		unordered_map<string, string> map;

		for (int j = 0; j < qry.column_count(); ++j) {
			map[qry.column_name(j)] = (*i).get<char const*>(j);
		}

		vec.push_back(map);
	}

	return vec;
}