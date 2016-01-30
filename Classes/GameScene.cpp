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
#include "MyTime.h"
#include "Utils.h"
#include "Dabaojian.h"
#include "i18n.h"
#include "Shi.h"

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

	CCASSERT(Director::getInstance()->getVisibleOrigin().length() < std::numeric_limits<float>::epsilon(), "WTF is cocos2dx doing");

	SQLUtils::createTable();

	addJoystick();
	addSettingButton();
	addActionButton();
	addLabels();

	_chunkManager = ChunkManager::getInstance();
	_chunkManager->setLayer(_holder);

	_spriteManager = SpriteManager::getInstance();
	_spriteManager->setLayer(_holder);
	//to do: register all creat functions.
	_spriteManager->registerCreateFunc("Human", [] { return Human::create(); });
	_spriteManager->registerCreateFunc("Dabaojian", [] {return Dabaojian::create(); });
	_spriteManager->registerCreateFunc("Shi", [] {return Shi::create(); });

	_spriteManager->registerCreateFuncWithJson("Statue", [](const rapidjson::Document& json) { return Statue::createWithJson(json); });

	auto chunkListener = EventListenerCustom::create(ChunkJoinWorldEvent::getName(),
		CC_CALLBACK_1(SpriteManager::onChunkCreated, _spriteManager));
	_eventDispatcher->addEventListenerWithFixedPriority(chunkListener, 1);
	
	chunkListener = EventListenerCustom::create(ChunkDisjoinWorldEvent::getName(),
		CC_CALLBACK_1(SpriteManager::onChunkRemoved, _spriteManager));
	_eventDispatcher->addEventListenerWithFixedPriority(chunkListener, 1);

	auto mobDieListener = EventListenerCustom::create(MobDieEvent::getName(),
		CC_CALLBACK_1(SpriteManager::onMobDied, _spriteManager));
	_eventDispatcher->addEventListenerWithFixedPriority(mobDieListener, 1);

	auto moveListener = EventListenerCustom::create(YourMoveEvent::getName(),
		CC_CALLBACK_1(SpriteManager::onYourMove, _spriteManager));
	_eventDispatcher->addEventListenerWithFixedPriority(moveListener, 1);

	_chunkManager->updateChunks(Point(1, 2));


	initYou(Vec2(getVisibleSize().width / 2,
		         getVisibleSize().height / 2));




	//add move listener
	auto mListener = EventListenerCustom::create(YourMoveEvent::getName(), [=](EventCustom* event){
		auto moveEvent = static_cast<YourMoveEvent*>(event->getUserData());
	//	log("--------------got move event, x=%f, y=%f", moveEvent->offset.x, moveEvent->offset.y);
		auto offset = moveEvent->offset;
		_holder->setPosition(_holder->getPosition() - offset);


		auto pos = _you->getPosition();
		_posLabel->setString("x: "+to_string(pos.x)+" y: "+to_string(pos.y));

		for (int x = -1; x < 2; x++) {
			for (int y = -1; y < 2; y++) {
				Vec2 vec(x / 2.0f * ChunkManager::CHUNK_WIDTH, y / 2.0f * ChunkManager::CHUNK_HEIGHT);
				_chunkManager->updateChunks(pos + vec);
			}
		}
	});
	_eventDispatcher->addEventListenerWithFixedPriority(mListener, 1);

	schedule(schedule_selector(GameScene::updateWorld), 1.0f/24);
	MyTime::getInstance()->setRealMsec(0); // to do: read from database;

	return true;
}

void GameScene::updateWorld(float dt){
	if (isPaused()) return;

	auto time = MyTime::getInstance();
	time->addRealMsec(dt * 1000);
	int hour = time->getVirtualHour();
	_timeLabel->setString(R::getString(R::HOUR)+": "+to_string(hour));

	_HPLabel->setString(R::getString(R::HP)+ ": "+to_string(_you->getHP()));

	//add mobs
	if (hour > 22 || hour < 8 && time->toRealSec() % 20 == 0) {
		time->addRealMsec(1000); // to do
		auto sp = _spriteManager->createSprite("Human");
		_holder->addChild(sp, JOYSTICK);
		auto m = dynamic_cast<Mob*>(sp);
		CC_ASSERT(m);
		sp->setPosition(_you->getPosition()+ 200 * randomVector(time->toRealSec()));
	}

	Vec2 vel(5, 5);
	vel.scale(_joystick->getGradientVector());

	_you->setPosition(_you->getPosition() + vel );
}

void GameScene::initYou(const Point& pos){
	_you = You::getInstance();
	assert(_you);
	_you->setPosition(pos);

	_holder->addChild(_you, YOU);
}

GameScene::~GameScene() {
	CC_SAFE_DELETE(_spriteManager);
	CC_SAFE_DELETE(_chunkManager);
}

void GameScene::addSettingButton() {
	using namespace cocos2d::ui;

	auto button = Button::create("setting.png");
	assert(button);
	button->addTouchEventListener([this](Ref *pSender, cocos2d::ui::Widget::TouchEventType type) {
		if (Widget::TouchEventType::BEGAN == type) {
			if (_isPaused) {
				CC_ASSERT(_inventory->getReferenceCount() == 1);
				_inventory->removeFromParent();
				_inventory = nullptr;
				resumeLayer();
			}
			else {
				_inventory = _you->showInventory();
				_inventory->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
				Scene::addChild(_inventory, JOYSTICK);
				_inventory->setPosition(getVisibleSize() / 2);
				pauseLayer();
			}
		}
	});
	
	auto screen = Director::getInstance()->getVisibleSize();
	auto size = button->getContentSize();
	button->setPosition(Point(size.width + 20, screen.height - size.height));

	Scene::addChild(button, JOYSTICK);
}

void GameScene::addActionButton()
{
	auto button = cocos2d::ui::Button::create("blade.png");
	assert(button);
	button->addTouchEventListener([this](Ref *pSender, cocos2d::ui::Widget::TouchEventType type) {
		if (isPaused()) return;
		using namespace cocos2d::ui;
		if (Widget::TouchEventType::BEGAN == type) {
			_you->startAttacking();
		}
	});
	button->setPosition(Point(getVisibleSize().width - 40, 50));
	Scene::addChild(button, JOYSTICK);
}

void GameScene::addJoystick() {
	_joystick = Joystick::create();
	assert(_joystick);
	Scene::addChild(_joystick, ZOrder::JOYSTICK);

	_freeArrow = FreeArrow::create();
	CC_ASSERT(_freeArrow);
	Scene::addChild(_freeArrow, ZOrder::JOYSTICK - 1); //to do?
}

void GameScene::addLabels()
{
	_posLabel = Label::create();
	Scene::addChild(_posLabel);
	_posLabel->setPosition(getVisibleSize().width / 2, getVisibleSize().height * 0.9f);
	_timeLabel = Label::create();
	Scene::addChild(_timeLabel);
	_timeLabel->setPosition(getVisibleSize().width / 2, getVisibleSize().height * 0.85f);
	_HPLabel = Label::create();
	Scene::addChild(_HPLabel);
	_HPLabel->setPosition(getVisibleSize().width / 2, getVisibleSize().height * 0.8f);
}

GameScene::GameScene():
	_isPaused(false),
	_inventory(nullptr),
	_you(nullptr),
	_joystick(nullptr),
	_freeArrow(nullptr),
	_posLabel(nullptr),
	_timeLabel(nullptr),
	_HPLabel(nullptr)
{
}
