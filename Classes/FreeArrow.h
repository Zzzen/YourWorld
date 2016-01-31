#pragma once

#include "cocos2d.h"

USING_NS_CC;

class FreeArrow : public Node {
public:
	static FreeArrow* create();

protected:
	FreeArrow();

	void setCircleColor(Color4F color) { _circleColor = color; }
	bool init() override;
	// whether tracking an touch when it begins.
	virtual bool isPointAccepted(const Point& pos);

	void onTouchesBegan(const std::vector<Touch*>& touches, Event  *event);
	void onTouchesMoved(const std::vector<Touch*>& touches, Event  *event);
	void onTouchesEnded(const std::vector<Touch*>& touches, Event  *event);
	void onTouchesCancelled(const std::vector<Touch*>& touches, Event  *event) { onTouchesEnded(touches, event); }

	// default angle is 90 degree
	Sprite* _arrow;
	DrawNode* _drawNode;
	Color4F _circleColor;
	EventListenerTouchAllAtOnce* _touchListener;

private:
	int _touchID;
	bool _isTouched;

};