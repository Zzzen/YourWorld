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

	initKeyboardListener();

	initInputLayer();

	initRecordLayer();

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

	auto bg = Sprite::create("bg.png");
	addChild(bg, -1);
	bg->setPosition(winSize/2);
	bg->setScale(max(winSize.width / bg->getContentSize().width, winSize.height / bg->getContentSize().height));

	return true;
}

void StartScene::initKeyboardListener()
{
	auto listener = EventListenerKeyboard::create();
	listener->onKeyReleased = [this](cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event) {
		if (keyCode == EventKeyboard::KeyCode::KEY_BACK || keyCode == EventKeyboard::KeyCode::KEY_ESCAPE )
		{
			this->onBackButtonClicked();
		}
	};
	
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void StartScene::initInputLayer()
{
	_inputLayer = LayerColor::create(Color4B(255,0,255,180), getContentSize().width, getContentSize().height);
	_inputLayer->setVisible(false);
	addChild(_inputLayer, 100);

	auto swallower = EventListenerTouchOneByOne::create();
	swallower->onTouchBegan = [this](Touch*, Event*) { return _inputLayer->isVisible(); };
	swallower->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(swallower, _inputLayer);

	_input = TextFieldTTF::textFieldWithPlaceHolder(R::getString(R::INPUT_NAME),
		"",
		24);
	_inputLayer->addChild(_input);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	// on android, TextFieldTTF cannot auto adjust its position when soft-keyboard pop up
	// so we had to set a higher position to make it visable
	_input->setPosition(Vec2(getContentSize().width / 2, getContentSize().height / 2 + 50));
#else
	_input->setPosition(Vec2(getContentSize().width / 2, getContentSize().height / 2));
#endif

	auto yes = TextButton::create();
	yes->onTouched = [this]() {
		_input->detachWithIME();

		auto name = _input->getString();
		//sanitize input
		name.erase(remove_if(name.begin(), name.end(), ::isspace), name.end());

		if (name.size() == 0) {
			log("invalid name");
			return;
		}

		if (FileUtils::getInstance()->isFileExist(getRecordFileName())) {
			auto records = FileUtils::getInstance()->getStringFromFile(getRecordFileName());
			_record.Parse(records.c_str());

			for (auto it = _record.Begin(); it != _record.End(); it++) {
				if (name == it->GetString()) {
					log("database name has been added.");
					return;
				}
			}
		}
		else {
			_record.SetArray();
		}

		_record.PushBack(rapidjson::Value(name.c_str(), _record.GetAllocator()), _record.GetAllocator());

		FileUtils::getInstance()->writeStringToFile(jsonToString(_record), getRecordFileName());

		startGameScene(name);
	};
	yes->setText(R::getString(R::CONFIRM_INPUT));
	yes->setPosition(getVisibleSize().width / 2.0f, getVisibleSize().height / 2.0f - 40);
	_inputLayer->addChild(yes);

	auto no = TextButton::create();
	no->setText("NO");
	no->onTouched = [this]() {
		_inputLayer->setVisible(false);
		_input->detachWithIME();
		_input->setString("");
	};
	no->setPosition(yes->getPosition() - Vec2(0, 30));
	_inputLayer->addChild(no);
}

void StartScene::initRecordLayer()
{
	_recordLayer = LayerColor::create(Color4B(255, 0, 255, 180), getContentSize().width, getContentSize().height);
	_recordLayer->setVisible(false);
	addChild(_recordLayer, 100);

	auto swallower = EventListenerTouchOneByOne::create();
	swallower->onTouchBegan = [this](Touch*, Event*) { return _recordLayer->isVisible(); };
	swallower->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(swallower, _recordLayer);
}

void StartScene::onLoadButtonClicked()
{
	auto fileData = FileUtils::getInstance()->getStringFromFile( getRecordFileName() );
	if (fileData.size()==0) {
		return;
	}

	_recordLayer->setVisible(true);
	_frontNode = _recordLayer;

	static const string SCROLL_VIEW = "SCOLLVIEW";
	if (_recordLayer->getChildByName(SCROLL_VIEW)) {
		_recordLayer->removeChildByName(SCROLL_VIEW);
	}

	auto view = ScrollView::create();
	view->setPosition(Vec2(100, 100));
	_recordLayer->addChild(view);

	view->setContentSize(Size(getVisibleSize().width * 3.0f / 5.0f, getVisibleSize().height / 2.0f));
	int index = 0;

	_record.Parse(fileData.c_str());

	assert(_record.IsArray());
	for (auto it = _record.Begin(); it != _record.End(); it++)
	{
		//log("records.json :  %s", it->GetString());

		string name = it->GetString();

		auto saved = TextButton::create();
		saved->setText(name);
		saved->onTouched = [this, name]() {
			startGameScene(name);
		};

		auto dlt = TextButton::create();
		dlt->setText(R::getString(R::DELETE));
		dlt->onTouched = [this, it, saved, dlt, name]() {
			_record.Erase(it);

			auto utils = FileUtils::getInstance();
			utils->writeStringToFile(jsonToString(_record), getRecordFileName());
			utils->removeFile(utils->getWritablePath() + name + ".sqlite3");

			saved->removeFromParent();
			dlt->removeFromParent();
		};

		view->addChild(saved);
		view->addChild(dlt);
		saved->setPosition(view->getContentSize().width / 2.0f, index * 32 + 16);
		dlt->setPosition(view->getContentSize().width * 2.f / 3.f, index * 32 + 16);

		index++;
	}

	view->setInnerContainerSize(Size(getVisibleSize().width * 3.0f / 5.0f + 0.01f, index * 32 + 16));
}

void StartScene::onNewGameButtonClicked()
{
	_frontNode = _inputLayer;
	_inputLayer->setVisible(true);
	_input->attachWithIME();
	_input->setString("");
}

void StartScene::onBackButtonClicked()
{
	if (_frontNode && _frontNode->isVisible()) {
		_frontNode->setVisible(false);
	}
	else {
		Director::getInstance()->end();
	}
}

void StartScene::startGameScene(const string & dbName)
{
	SQLUtils::setDatabaseFileName(dbName + ".sqlite3");

	auto game = GameScene::create();
	auto trans = TransitionFadeBL::create(1.0f, game);

	Director::getInstance()->replaceScene(trans);
}

StartScene::StartScene() :
	_input(nullptr),
	_frontNode(nullptr),
	_inputLayer(nullptr),
	_recordLayer(nullptr)
{
}
