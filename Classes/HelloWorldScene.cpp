#include "HelloWorldScene.h"
#include "ChunkManager.h"
#include "Joystick.h"

#include <thread>

using namespace cocos2d;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);


	auto joystick = Joystick::create();
	scene->addChild(joystick, 100);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = Label::createWithTTF("Your World", "fonts/Marker Felt.ttf", 24);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
    
	//GradientVectors vecs{ Vec2(1, -1), Vec2(3, 5), Vec2(12, 5), Vec2(-12, -4) };
	//_chunk = Chunk::create(vecs);
	//_chunk->setPosition(Vec2(0, 0));
	//addChild(_chunk);
	auto manager = new ChunkManager(this);
	manager->updateChunks(Point(1, 2));
	manager->updateChunks(Point(4, 4));

	auto _listener = EventListenerCustom::create(JoystickEvent::EVENT_JOYSTICK, [=](EventCustom* event){
		JoystickEvent* jsevent = static_cast<JoystickEvent*>(event->getUserData());
		log("--------------got joystick event, %p,  angle=%f", jsevent, jsevent->mAnagle);

		// do your business you'd like to
	}); 

	_eventDispatcher->addEventListenerWithFixedPriority(_listener, 1);

	//addTouchListener();

    return true;
}

void HelloWorld::addTouchListener(){
	auto listener = EventListenerMouse::create();
	
	_touching = false;
	listener->onMouseDown = [this](Event* event){
		auto mouse = (EventMouse*)event;
		_touching = true;
		_touchPoint = mouse->getLocation();
	};
	listener->onMouseMove = [this](Event* event){
		if (!_touching) return;

		CCLOG("touching");
		auto mouse = (EventMouse*)event;
		auto pos = mouse->getLocation();
		auto offset = _touchPoint - pos;
		offset.y = -offset.y;

		setPosition(getPosition() - offset);
		_touchPoint = pos;
	};
	listener->onMouseUp = [this](Event* event){
		_touching = false;
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
