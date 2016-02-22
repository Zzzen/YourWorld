#pragma once

#include <functional>

#include "cocos2d.h"

USING_NS_CC;

/*
	@class DialogLayer
	@brief subclass of LayerColor that swallows touch events.

*/

class DialogLayer : public LayerColor {
public:
	static DialogLayer* create(const Color4B& color);

	std::function<void()> onShowed;
	std::function<void()> onHidden;

	void setVisible(bool visible) override;

	DialogLayer();
protected:
	void initListeners();
};