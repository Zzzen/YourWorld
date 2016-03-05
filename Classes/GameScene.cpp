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
#include "TextButton.h"
#include "Cao.h"
#include "Jian.h"
#include "LiZhi.h"
#include "LiZhiSeed.h"
#include "Boulder.h"
#include "Rock.h"

USING_NS_CC;

#define REGISTER_CREATE(manager, Type) manager->registerCreateFunc( #Type , [] { return Type::create(); })
#define REGISTER_CREATE_JSON(manager, Type) manager->registerCreateFuncWithJson( #Type , [](const rapidjson::Document& json) { return Type::createWithJson(json); })

static const int FPS = 24;

enum ZOrder
{
	YOU = 200,
	JOYSTICK = 100
};

bool GameScene::init(){
	if (!Scene::initWithPhysics()){
		return false;
	}
	getPhysicsWorld()->setGravity(Vec2::ZERO);
	getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

	_holder = Layer::create();
	assert(_holder);
	Scene::addChild(_holder);

//	CCASSERT(Director::getInstance()->getVisibleOrigin().length() < std::numeric_limits<float>::epsilon(), "WTF is cocos2dx doing");

	SQLUtils::createTable();

	addJoystick();
	addSettingButton();
	addActionButton();
	addLabels();

	auto saveBtn = TextButton::create();
	saveBtn->setText(R::getString(R::SAVE));
	Scene::addChild(saveBtn);
	saveBtn->setPosition(getVisibleSize().width - 40, getVisibleSize().height - 40);
	//to do: add everything to cache first
	saveBtn->onTouched = []() { 
		auto sprites = SpriteManager::getInstance()->getAllSprites();
		vector<SerializableSprite*> spritesWithImproperId;
		for (const auto& sp : sprites) {
			SQLUtils::addToCache(sp);
			if (sp->getRowid() < 0) {
				spritesWithImproperId.push_back(sp);
			}
		}
		auto& updatedRowids = SQLUtils::flush();
		for (auto& sp : spritesWithImproperId) {
			log("update rowid: %lld -> %lld", sp->getRowid(), updatedRowids[sp->getRowid()]);
			sp->setRowid(updatedRowids[sp->getRowid()]);
		}
	};

	_chunkManager = ChunkManager::getInstance();
	_chunkManager->setLayer(_holder);

	_spriteManager = SpriteManager::getInstance();
	_spriteManager->setLayer(_holder);
	//to do: register all creat functions.
	REGISTER_CREATE(_spriteManager, Human);
	REGISTER_CREATE(_spriteManager, Dabaojian);
	REGISTER_CREATE(_spriteManager, Shi);
	REGISTER_CREATE(_spriteManager, Cao);
	REGISTER_CREATE(_spriteManager, LiZhi);
	REGISTER_CREATE(_spriteManager, LiZhiSeed);
	REGISTER_CREATE(_spriteManager, Boulder);
	REGISTER_CREATE(_spriteManager, Rock);

	REGISTER_CREATE_JSON(_spriteManager, Statue);
	REGISTER_CREATE_JSON(_spriteManager, Human);
	REGISTER_CREATE_JSON(_spriteManager, Dabaojian);
	REGISTER_CREATE_JSON(_spriteManager, Shi);
	REGISTER_CREATE_JSON(_spriteManager, Cao);
	REGISTER_CREATE_JSON(_spriteManager, LiZhi);
	REGISTER_CREATE_JSON(_spriteManager, LiZhiSeed);
	REGISTER_CREATE_JSON(_spriteManager, Boulder);
	REGISTER_CREATE_JSON(_spriteManager, Rock);

	schedule([this](float dt) {
		_spriteManager->updateVas(dt);
	}, 0.3f, "Update");

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

	//_chunkManager->updateChunks(Point(1, 2));


	initYou(Vec2(getVisibleSize().width / 2,
		         getVisibleSize().height / 2));

	scheduleOnce([this](float dt) { _you->setPosition(_you->getPosition()+Vec2(1.0f, 0.f)); }, 2.f, "initial movement");
	_holder->setPosition( - _you->getPosition() + getVisibleSize() / 2);

	//add move listener
	auto mListener = EventListenerCustom::create(YourMoveEvent::getName(), [=](EventCustom* event){
		auto moveEvent = static_cast<YourMoveEvent*>(event->getUserData());
	//	log("--------------got move event, x=%f, y=%f", moveEvent->offset.x, moveEvent->offset.y);
		auto offset = moveEvent->offset;

		auto pos = _you->getPosition();

		_holder->setPosition( - pos + getVisibleSize() / 2);

		_posLabel->setString("x: "+to_string(pos.x)+" y: "+to_string(pos.y));
		
		for (int x = -1; x < 2; x++) {
			for (int y = -1; y < 2; y++) {
				Vec2 vec(x / 2.0f * ChunkManager::CHUNK_WIDTH, y / 2.0f * ChunkManager::CHUNK_HEIGHT);
				_chunkManager->updateChunks(pos + vec);
			}
		}
	});
	_eventDispatcher->addEventListenerWithFixedPriority(mListener, 1);

	schedule(schedule_selector(GameScene::updateWorld), 1.0f/FPS);

	return true;
}

void GameScene::updateWorld(float dt){
	if (isPaused()) return;

	auto time = MyTime::getInstance();
	time->addRealMsec(dt * 1000);
	int hour = time->getVirtualHour();
	_timeLabel->setString(R::getString(R::HOUR)+": "+to_string(hour));

	_HPLabel->setString(R::getString(R::HP)+ ": "+to_string(_you->getHP()));

	//add mobs at night
	//if (hour > 22 || hour < 8) {
		auto fre = 1.f / dt;
		float p = 1.f / (5 * fre);
		if (RandomHelper::random_real(0.f, 1.f) < p) {
			auto sp = _spriteManager->createSprite("Human");
			_holder->addChild(sp, JOYSTICK);
			auto m = dynamic_cast<Mob*>(sp);
			CC_ASSERT(m);
			sp->setPosition(_you->getPosition() + 200 * Vec2::forAngle(RandomHelper::random_real(0.f, 3.14159f)));
		}
	//}

	Vec2 vel(5, 5);
	vel.scale(_joystick->getGradientVector());

	_you->setPosition(_you->getPosition() + vel );
}

void GameScene::initYou(const Point& pos){
	_you = You::getInstance();
	assert(_you);
	_you->setPosition(pos);

	auto label = _you->getItemLabel();
	addChild(label, ZOrder::JOYSTICK);

	auto map = SQLUtils::selectYou();
	if (map.size() != 0) {
		auto x = strTo<int>(map["x"]);
		auto y = strTo<int>(map["y"]);
		_you->setPosition(x, y);

		_you->setRowid(strTo<int64_t>(map["rowid"]));

		Document doc;
		doc.Parse(map["properties"].c_str());
		_you->initWithJson(doc);
	}

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
	_freeArrow->onTouchEnded = [this](const Vec2& vec) {
		auto j = Jian::create(vec.getAngle(), 100, 3, 10, _you);
		j->setPosition(_you->getPosition());
		j->start();
		_holder->addChild(j, ZOrder::YOU);
	};

	//schedule([this](float) {
	//	for (int i = 0; i < 360; i += 30) {
	//		auto j = Jian::create(MATH_DEG_TO_RAD(i), 100, 3, 10, _you);
	//		j->setPosition(_you->getPosition());
	//		j->start();
	//		_holder->addChild(j, ZOrder::YOU);
	//	}
	//}, 1.3f, "feafsegh");
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
