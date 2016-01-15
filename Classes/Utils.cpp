#include "Utils.h"

using namespace std;
USING_NS_CC;

string str(const Point& point) {
	string s = " Point: ";
	return s + to_string(point.x) + ", " + to_string(point.y);
}

string str(const Size& size) {
	string s = " Size: ";
	return s + to_string(size.width) + ", " + to_string(size.height);
}

string str(const Rect& rect) {
	string s = " Rect: ";
	return s + str(rect.origin) + str(rect.size);
}


void setNodePauseRecursively(Vector<Node*>& nodes, bool pause)
{
	if (pause) {
		for (auto node : nodes) {
			node->pause();
			setNodePauseRecursively(node->getChildren(), pause);
		}
	}
	else {
		for (auto node : nodes) {
			node->resume();
			setNodePauseRecursively(node->getChildren(), pause);
		}
	}
}
