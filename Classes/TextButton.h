#pragma once

#include "cocos2d.h"

#include <functional>

USING_NS_CC;

class TextButton : public Node {
public:
	enum SizePolicy {
		FIXED,
		RELATIVE
	};

	CREATE_FUNC(TextButton);

	std::function<void()> onTouched;

	Rect getBoundingBox()const override;

	void setText(std::string text) { _label->setString(text); onTextChanged(); }

	void setFontSize(float size) { _label->setSystemFontSize(size);  onTextChanged(); }
	void setSystemFontName(std::string fontName) { _label->setSystemFontName(fontName); onTextChanged(); }
	void setTextColor(Color4B color) { _label->setTextColor(color); }

	void setBackgroudSize(const Size& size);

	TextButton();
	~TextButton() override;

	CC_SYNTHESIZE(int, _segment, Segment);
	CC_SYNTHESIZE(int, _radius, Radius);
	CC_SYNTHESIZE(Color4F, _backgroudColor, BackgroudColor);

protected:
	bool init() override;

	virtual void onTouchBegan();
	virtual void onTouchEnded();

	virtual void onTextChanged();

	Label* _label;
	DrawNode* _drawNode;
	EventListenerTouchOneByOne* _touchListener;
	SizePolicy _sizePolicy;
};