#pragma once

#include "cocos2d.h"

USING_NS_CC;
using namespace std;

string str(const Point& point);

string str(const Size& size);

string str(const Rect& rect);

void setNodePauseRecursively(Vector<Node*>& nodes, bool paused);

Vec2 randomVector(float radian);

Size getVisibleSize();

class Xu {
public:
	Xu(Ref* ref) : _ref(ref) { _ref->retain(); }
	~Xu() { _ref->release(); }

	Ref* _ref;
};