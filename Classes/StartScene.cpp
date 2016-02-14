#include "StartScene.h"

#include "GameScene.h"
#include "TextButton.h"
#include "i18n.h"
#include "Utils.h"
#include "SQLUtils.h"

#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

#define RAPIDJSON_HAS_STDSTRING 1
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"


using namespace cocos2d;
using namespace cocostudio;
using namespace ui;

using namespace rapidjson;


static string getRecordFileName() {
	auto utils = FileUtils::getInstance();
	auto path = utils->getWritablePath();

	return path + "records.json";
}


StartScene * StartScene::create()
{
	auto s = new StartScene();
	if (s->init()) {
		s->autorelease();
		return s;
	}
	else {
		CC_SAFE_DELETE(s);
		return nullptr;
	}
}

bool StartScene::init()
{
	if (!Scene::init()) return false;

	auto winSize = getContentSize();

	auto load = TextButton::create();
	load->onTouched = [this]() { onLoadButtonClicked(); };
	load->setText(R::getString(R::LOADGAME));
	load->setPosition(winSize.width/2.0f, winSize.height / 2.0f);
	addChild(load);

	auto start = TextButton::create();
	start->onTouched = [this]() { onNewGameButtonClicked(); };
	start->setText(R::getString(R::STARTNEWGAME));
	start->setPosition(winSize.width / 2.0f, winSize.height / 2.0f - 20);
	addChild(start);

	auto setting = TextButton::create();
	setting->onTouched = []() {};
	setting->setText(R::getString(R::SETTING));
	setting->setPosition(winSize.width / 2.0f, winSize.height / 2.0f - 40);
	addChild(setting);

	auto yes = TextButton::create();
	yes->onTouched = [this]() {
		if (_input){
			auto name = _input->getString();

			if (name.size() == 0) {
				log("wrong name");
				return;
			}

			rapidjson::Document doc;

			if (FileUtils::getInstance()->isFileExist(getRecordFileName())) {
				auto records = FileUtils::getInstance()->getStringFromFile(getRecordFileName());
				doc.Parse(records.c_str());
				
				for (auto it = doc.Begin(); it != doc.End(); it++) {
					if (name == it->GetString()) {
						log("it has been added.");
						return;
					}
				}
			}
			else {
				doc.SetArray();
			}

			doc.PushBack(rapidjson::Value(name.c_str(), doc.GetAllocator()), doc.GetAllocator());

			FileUtils::getInstance()->writeStringToFile(jsonToString(doc), getRecordFileName());

			startGameScene(name);
		}
	};
	yes->setText("Yes");
	yes->setPosition(winSize.width / 2.0f, winSize.height / 2.0f - 80);
	addChild(yes);


	auto bg = Sprite::create("bg.png");
	addChild(bg, -1);
	bg->setPosition(winSize/2);
	bg->setScale(max(winSize.width / bg->getContentSize().width, winSize.height / bg->getContentSize().height));

	return true;
}


void StartScene::onLoadButtonClicked()
{
	auto records = FileUtils::getInstance()->getStringFromFile( getRecordFileName() );
	if (records.size()==0) {
		return;
	}

	{
		auto view = ScrollView::create();
		view->setBackGroundColor(Color3B::RED);
		view->setPosition(Vec2(100,100));
		addChild(view);

		view->setContentSize(Size(getVisibleSize().width * 3.0f / 5.0f, getVisibleSize().height / 2.0f));
		int index = 0;

		Document doc;
		doc.Parse(records.c_str());

		assert(doc.IsArray());
		for (auto it = doc.Begin(); it != doc.End(); it++)
		{
			log("records.json :  %s", it->GetString());

			string name = it->GetString();

			auto saved = TextButton::create();
			saved->setText(name);
			saved->onTouched = [this, name]() {
				startGameScene(name);
			};

			view->addChild(saved);
			saved->setPosition(view->getContentSize().width / 2.0f, index*32 + 16);
			index++;
		}

		view->setInnerContainerSize(Size(getVisibleSize().width * 3.0f / 5.0f + 0.01f, index * 32 + 16));
	}
}

void StartScene::onNewGameButtonClicked()
{
	if (_input) {
		log("text: %s", _input->getString().c_str());
		return;
	}

	_input = TextFieldTTF::textFieldWithPlaceHolder("<click here for input>",
		"",
		24);
	addChild(_input);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	// on android, TextFieldTTF cannot auto adjust its position when soft-keyboard pop up
	// so we had to set a higher position to make it visable
	_input->setPosition(Vec2(getContentSize().width / 2, getContentSize().height/2 + 50));
#else
	_input->setPosition(Vec2(getContentSize().width / 2, getContentSize().height / 2));
#endif

	_input->attachWithIME();

}

void StartScene::startGameScene(const string & dbName)
{
	SQLUtils::setDatabaseFileName(dbName + ".sqlite3");

	auto game = GameScene::create();
	auto trans = TransitionFadeBL::create(1.0f, game);

	Director::getInstance()->replaceScene(trans);
}
