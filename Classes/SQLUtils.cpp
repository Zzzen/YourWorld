#include "SQLUtils.h"

#include "lib/sqlite3pp/sqlite3pp.h"
#include "CCFileUtils.h"
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
	Document doc;
	auto& allocator = doc.GetAllocator();
	doc.SetObject();

	const auto& map = sprite->toJson();
	for (const auto& pair : map) {
		rapidjson::Value key(pair.first.c_str(), pair.first.size(), allocator);
		rapidjson::Value value(pair.second.c_str(), pair.second.size(), allocator);
		doc.AddMember(key, value, allocator);
	}

	StringBuffer sb;
	Writer<StringBuffer> writer(sb);

	doc.Accept(writer);
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
	cmd.binder() << x << y << className.c_str() << properties.c_str();
	cmd.execute();

	//log("insert msg: %s", db.error_msg());
}