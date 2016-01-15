#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "Utils.h"

USING_NS_CC;

class You;
class ChunkManager;
class SpriteManager;
class Joystick;
class GameScene : public Scene
{
public:
	bool init() override;

	// implement the "static create()" method manually
	CREATE_FUNC(GameScene);

	void pauseLayer() { _isPaused = true; setNodePauseRecursively(_holder->getChildren(), true); }

	void resumeLayer() { _isPaused = false; setNodePauseRecursively(_holder->getChildren(), false); }

	inline bool isPaused() { return _isPaused; }

	~GameScene();
protected:
	You* _you;
	Joystick* _joystick;
	Label* _posLabel;

	//now I know why `HelloWorldScene` is a `Layer` ܳ_ܳ
	Layer* _holder;


	//to do: use GameLayer?
	void addChild(Node* child, int zOrder) override { CCASSERT(false, "use _holder->addChild() or Scene::addChild()"); }
	void addChild(Node* child) override { CCASSERT(false, "use _holder->addChild() or Scene::addChild()"); }

	ChunkManager* _chunkManager;
	SpriteManager* _spriteManager;
	
	virtual void updateWorld(float dt);

	void initYou(const Point& pos);
	void addSettingButton();
	
	bool _isPaused;
};




#endif