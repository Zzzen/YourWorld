#include "DialogLayer.h"

DialogLayer * DialogLayer::create(const Color4B & color)
{

	auto layer = new DialogLayer();
	
	auto ss = Director::getInstance()->getWinSize();

	if (layer->initWithColor(color, ss.width, ss.height)) {
		layer->initListeners();
		layer->autorelease();
		
		return layer;
	}
	else {
		CC_SAFE_DELETE(layer);
		return nullptr;
	}
}

void DialogLayer::setVisible(bool visible)
{
	if (visible == isVisible()) return;

	LayerColor::setVisible(visible);

	if (visible && onShowed) {
		onShowed();
	}

	if (!visible && onHidden) {
		onHidden();
	}
}

DialogLayer::DialogLayer():
	onShowed(nullptr),
	onHidden(nullptr)
{
}

void DialogLayer::initListeners()
{
	//auto listener = EventListenerKeyboard::create();
	//listener->onKeyReleased = [this](cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event) {
	//	if (!isVisible()) return;

	//	if (keyCode == EventKeyboard::KeyCode::KEY_BACK || keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
	//	{
	//		setVisible(false);
	//	}
	//};
	//_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	auto swallower = EventListenerTouchOneByOne::create();
	swallower->onTouchBegan = [this](Touch*, Event*) { return isVisible(); };
	swallower->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(swallower, this);
}
