#ifndef __YOU_H__
#define __YOU_H__

#include "cocos2d.h"

USING_NS_CC;

class YourMoveEvent : public Ref{
private:
	YourMoveEvent(){}

public:
	~YourMoveEvent() override {}
	bool init(){ return true; }
	CREATE_FUNC(YourMoveEvent);

public:
	static const std::string EVENT_YOURMOVE;
	Point offset;
};


class You : public Sprite{
public:
	CREATE_FUNC(You);

	void setPosition(const Vec2& position) override;
protected:
	bool init() override;
	You(){}
};


#endif