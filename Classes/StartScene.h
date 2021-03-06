#pragma once

#include "DialogLayer.h"

#include "cocos2d.h"


#define RAPIDJSON_HAS_STDSTRING 1
#include "json/document.h"


USING_NS_CC;

class StartScene : public Scene, public IMEDelegate {
public:
	static StartScene* create();

private:
	bool init() override;

	void initKeyboardListener();
	void initInputLayer();
	void initRecordLayer();

	void onLoadButtonClicked();
	void onNewGameButtonClicked();

	void onBackButtonClicked();

	void startGameScene(const std::string& dbName);

	Node* _frontNode;

	DialogLayer* _inputLayer;
	TextFieldTTF* _input;

	DialogLayer* _recordLayer;

	rapidjson::Document _record;

	StartScene();
};