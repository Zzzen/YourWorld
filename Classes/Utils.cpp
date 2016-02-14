#include "Utils.h"

using namespace std;
USING_NS_CC;


void showToast(string text)
{
	static int pRandom = 0;
	pRandom = (pRandom + 1) % 5;

	auto label = Label::createWithSystemFont(text, "", 16);
	label->setPosition(0, getVisibleSize().height - 30 * pRandom);

	auto scene = Director::getInstance()->getRunningScene();
	if (!scene) return;
	scene->addChild(label, 128, "Toast");

	auto moveTo = MoveTo::create(4.0f, Vec2(getVisibleSize().width, label->getPosition().y));
	label->runAction(moveTo);

	label->scheduleOnce([label](float) {label->removeFromParent(); }, moveTo->getDuration(), "label"); //to do
}



string jsonToString(const rapidjson::Document& doc) {
	using namespace rapidjson;
	StringBuffer sb;
	Writer<StringBuffer> writer(sb);
	doc.Accept(writer);
	return sb.GetString();
}

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

Size getVisibleSize() {
	return Director::getInstance()->getVisibleSize();
}

Vec2 randomVector(float radian) {
	return Vec2(cos(radian), sin(radian));
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
