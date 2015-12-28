#include "GameScene.h"
#include "Joystick.h"
#include "ChunkManager.h"
#include "You.h"
USING_NS_CC;

enum ZOrder
{
	YOU = 200,
	JOYSTICK = 100
};

bool GameScene::init(){
	if (!Scene::init()){
		return false;
	}

	_holder = Layer::create();
	addChild(_holder);

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	initYou(Vec2(origin.x + visibleSize.width / 2,
				 origin.y + visibleSize.height / 2));

	//add Joystick
	_joystick = Joystick::create();
	addChild(_joystick, ZOrder::JOYSTICK);


	auto manager = new ChunkManager(_holder);
	manager->updateChunks(Point(1, 2));
	manager->updateChunks(Point(4, 4));

	//add move listener
	auto mListener = EventListenerCustom::create(YourMoveEvent::EVENT_YOURMOVE, [=](EventCustom* event){
		auto moveEvent = static_cast<YourMoveEvent*>(event->getUserData());
	//	log("--------------got move event, x=%f, y=%f", moveEvent->offset.x, moveEvent->offset.y);
		_holder->setPosition(_holder->getPosition() - moveEvent->offset);

		manager->updateChunks(_you->getPosition());
	});
	_eventDispatcher->addEventListenerWithFixedPriority(mListener, 1);


	schedule(schedule_selector(GameScene::updateWorld), 1.0f/30);

	return true;
}

void GameScene::updateWorld(float dt){
	Vec2 vel(5, 5);
	vel.scale(_joystick->getGradientVector());

	_you->setPosition(_you->getPosition() + vel);
}

void GameScene::initYou(const Point& pos){
	_you = You::create();
	_you->setPosition(pos);
	_you->setTexture("you.png");

	_holder->addChild(_you, YOU);
}