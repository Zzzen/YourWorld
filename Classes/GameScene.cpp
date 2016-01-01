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
	assert(_holder);
	addChild(_holder);

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	initYou(Vec2(origin.x + visibleSize.width / 2,
				 origin.y + visibleSize.height / 2));

	//add Joystick
	_joystick = Joystick::create();
	assert(_joystick);
	addChild(_joystick, ZOrder::JOYSTICK);


	_manager = new ChunkManager(_holder);
	_manager->updateChunks(Point(1, 2));
	_manager->updateChunks(Point(4, 4));

	//add move listener
	auto mListener = EventListenerCustom::create(YourMoveEvent::EVENT_YOURMOVE, [=](EventCustom* event){
		auto moveEvent = static_cast<YourMoveEvent*>(event->getUserData());
	//	log("--------------got move event, x=%f, y=%f", moveEvent->offset.x, moveEvent->offset.y);
		auto offset = moveEvent->offset;
		_holder->setPosition(_holder->getPosition() - offset);

		auto pos = _you->getPosition();
		for (int x = -1; x < 2; x++) {
			for (int y = -1; y < 2; y++) {
				Vec2 vec(x / 2.0f*visibleSize.width, y / 2.0f*visibleSize.height);
				_manager->updateChunks(pos + vec);
			}
		}
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
	assert(_you);
	_you->setPosition(pos);
	_you->setTexture("you.png");

	_holder->addChild(_you, YOU);
}