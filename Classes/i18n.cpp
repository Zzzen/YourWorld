#pragma once

#include "i18n.h"
#include "cocos2d.h"

#define RAPIDJSON_HAS_STDSTRING 1
#include "json/document.h"

using namespace std;
using namespace rapidjson;

USING_NS_CC;

static Document* instance = nullptr;

string R::getString(const char* key) {
	if (!instance) {
		auto code = Application::getInstance()->getCurrentLanguageCode();
		auto fileName = StringUtils::format("i18n/%s.json", code);
		auto fileUtils = FileUtils::getInstance();
		CC_ASSERT(fileUtils->isFileExist(fileName));

		auto string = fileUtils->getStringFromFile(fileName);
		instance = new Document;
		instance->Parse(string.c_str());
	}

	if (instance->HasMember(key)) {
		return (*instance)[key].GetString();
	}
	else {
		CCLOG("%s not found in i18n files", key);
		return key;
	}
}