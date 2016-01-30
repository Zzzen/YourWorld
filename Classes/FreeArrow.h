#pragma once

#include "cocos2d.h"

USING_NS_CC;

class FreeArrow : public Node {
public:
	static FreeArrow* create();

protected:
	FreeArrow();

	bool init() override;

	void onTouchesBegan(const std::vector<Touch*>& touches, Event  *event);
	void onTouchesMoved(const std::vector<Touch*>& touches, Event  *event);
	void onTouchesEnded(const std::vector<Touch*>& touches, Event  *event);
	void onTouchesCancelled(const std::vector<Touch*>& touches, Event  *event) { onTouchesEnded(touches, event); }


	Sprite* _arrow;
	DrawNode* _drawNode;
	EventListenerTouchAllAtOnce* _touchListener;

private:
	int _touchID;
	bool _isTouched;

};