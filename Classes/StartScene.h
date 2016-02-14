#pragma once

#include "cocos2d.h"

USING_NS_CC;

class StartScene : public Scene, public IMEDelegate {
public:
	static StartScene* create();

private:
	bool init() override;

	void onLoadButtonClicked();
	void onNewGameButtonClicked();

	void startGameScene(const std::string& dbName);

	TextFieldTTF* _input;
};