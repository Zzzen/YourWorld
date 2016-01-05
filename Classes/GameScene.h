#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"

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

	~GameScene();
protected:
	You* _you;
	Joystick* _joystick;
	Label* _posLabel;

	//now I know why `HelloWorldScene` is a `Layer` ܳ_ܳ
	Layer* _holder;
	ChunkManager* _chunkManager;
	SpriteManager* _spriteManager;
	
	virtual void updateWorld(float dt);

	void initYou(const Point& pos);
};




#endif