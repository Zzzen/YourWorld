#include "GameScene.h"
#include "Joystick.h"
#include "ChunkManager.h"
#include "You.h"
#include "Statue.h"
#include "SQLUtils.h"
#include "SpriteManager.h"
#include "ui\UIButton.h"
#include "ui\UIWidget.h"
#include "Human.h"

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
	Scene::addChild(_holder);

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	initYou(Vec2(origin.x + visibleSize.width / 2,
				 origin.y + visibleSize.height / 2));

	//add Joystick
	_joystick = Joystick::create();
	assert(_joystick);
	Scene::addChild(_joystick, ZOrder::JOYSTICK);

	//add action button
	auto button = cocos2d::ui::Button::create("blade.png");
	assert(button);
	button->addTouchEventListener([this](Ref *pSender, cocos2d::ui::Widget::TouchEventType type) {
		using namespace cocos2d::ui;
		if (Widget::TouchEventType::BEGAN == type) {
			_you->startAttacking();
		}
	});
	button->setPosition(Point(visibleSize.width-40,50));
	Scene::addChild(button, JOYSTICK);


	_chunkManager = ChunkManager::getInstance();
	_chunkManager->setLayer(_holder);

	_spriteManager = SpriteManager::getInstance();
	_spriteManager->setLayer(_holder);
	auto chunkListener = EventListenerCustom::create(ChunkJoinWorldEvent::getName(),
		CC_CALLBACK_1(SpriteManager::onChunkCreated, _spriteManager));
	_eventDispatcher->addEventListenerWithFixedPriority(chunkListener, 1);
	
	chunkListener = EventListenerCustom::create(ChunkDisjoinWorldEvent::getName(),
		CC_CALLBACK_1(SpriteManager::onChunkRemoved, _spriteManager));
	_eventDispatcher->addEventListenerWithFixedPriority(chunkListener, 1);

	_chunkManager->updateChunks(Point(1, 2));

	_posLabel = Label::create();
	Scene::addChild(_posLabel);
	_posLabel->setPosition(visibleSize.width/2, 300);

	SQLUtils::createTable();

	//add move listener
	auto mListener = EventListenerCustom::create(YourMoveEvent::getName(), [=](EventCustom* event){
		auto moveEvent = static_cast<YourMoveEvent*>(event->getUserData());
	//	log("--------------got move event, x=%f, y=%f", moveEvent->offset.x, moveEvent->offset.y);
		auto offset = moveEvent->offset;
		_holder->setPosition(_holder->getPosition() - offset);

		char buffer[1024];
		sprintf(buffer, "x: %f, y:%f", _you->getPosition().x, _you->getPosition().y);
		_posLabel->setString(buffer);


		auto pos = _you->getPosition();
		for (int x = -1; x < 2; x++) {
			for (int y = -1; y < 2; y++) {
				Vec2 vec(x / 2.0f*visibleSize.width, y / 2.0f*visibleSize.height);
				_chunkManager->updateChunks(pos + vec);
			}
		}
	});
	_eventDispatcher->addEventListenerWithFixedPriority(mListener, 1);


	schedule(schedule_selector(GameScene::updateWorld), 1.0f/24);

	return true;
}

void GameScene::updateWorld(float dt){
	Vec2 vel(5, 5);
	vel.scale(_joystick->getGradientVector());

	_you->setPosition(_you->getPosition() + vel);
}

void GameScene::initYou(const Point& pos){
	_you = You::getInstance();
	assert(_you);
	_you->setPosition(pos);

	_holder->addChild(_you, YOU);

	auto h = Human::create();
	_holder->addChild(h, YOU);
	h->setPosition(Point(123,312));
}

GameScene::~GameScene() {
	CC_SAFE_DELETE(_spriteManager);
	CC_SAFE_DELETE(_chunkManager);
}