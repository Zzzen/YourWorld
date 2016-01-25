#pragma once

#include "cocos2d.h"

USING_NS_CC;

class FreeArrow : public Node {
public:
	static FreeArrow* create();

protected:
	FreeArrow();

	bool init() override;
	bool onTouchBegan(Touch *touch, Event *unused_event);
	void onTouchMoved(Touch *touch, Event *unused_event);
	void onTouchEnded(Touch *touch, Event *unused_event);

	Sprite* _arrow;
	EventListenerTouchOneByOne* _touchListener;
};