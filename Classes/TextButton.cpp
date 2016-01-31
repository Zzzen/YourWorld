#include "TextButton.h"

#include <vector>
#include "Utils.h"

using namespace std;

Rect TextButton::getBoundingBox() const
{
	return RectApplyAffineTransform(_label->getBoundingBox(), getNodeToParentAffineTransform());
}

void TextButton::setBackgroudSize(const Size & size)
{
}

TextButton::TextButton():
	_label(nullptr),
	_drawNode(nullptr),
	_touchListener(nullptr),
	_sizePolicy(RELATIVE),
	onTouched([]{}),
	_segment(40),
	_radius(5),
	_backgroudColor(Color4F::GRAY)
{
}

TextButton::~TextButton()
{
}

bool TextButton::init()
{
	if (!Node::init()) return false;

	_drawNode = DrawNode::create();
	_drawNode->setPosition(Vec2::ZERO);
	addChild(_drawNode);

	_label = Label::create();
	_label->setPosition(Vec2::ZERO);
	addChild(_label);

	_touchListener = EventListenerTouchOneByOne::create();

	_touchListener->setSwallowTouches(true);
	_touchListener->onTouchBegan = [this](Touch* touch, Event*) {
		if (_label->getBoundingBox().containsPoint(convertTouchToNodeSpace(touch))) {
			onTouchBegan();
			return true;
		}
		else {
			return false;
		}
	};

	_touchListener->onTouchEnded = [this](Touch* touch, Event*) {
		onTouchEnded();

		if (_label->getBoundingBox().containsPoint(convertTouchToNodeSpace(touch))) {
			Xu xu(this);
			onTouched();
		}
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(_touchListener, this);

	return true;
}

void TextButton::onTouchBegan()
{
	setScale(1.1f);
}

void TextButton::onTouchEnded()
{
	setScale(1.0f);
}

void TextButton::onTextChanged()
{
	if (FIXED == _sizePolicy) return;

	_drawNode->clear();

	const float w = _label->getBoundingBox().size.width * 1.2f,
		h = _label->getBoundingBox().size.height * 1.1f,
		r = _radius;

	vector<Point> points;
	for (int i = 0; i <= _segment; i++) {
		Point center(w / 2.0f - r, h / 2.0f - r);
		points.push_back(center + r * Point::forAngle(M_PI_2*(1.0f - i / (float)_segment)));
	}
	for (int i = 0; i <= _segment; i++) {
		Point center(w / 2.0f - r, -h / 2.0f + r);
		points.push_back(center + r * Vec2::forAngle(-M_PI_2*i / (float)_segment));
	}
	for (int i = 0; i <= _segment; i++) {
		Point center(-w / 2.0f + r, -h / 2.0f + r);
		points.push_back(center + r* Vec2::forAngle(-M_PI_2*i / (float)_segment - M_PI_2));
	}
	for (int i = 0; i <= _segment; i++) {
		Point center(-w / 2.0f + r, h / 2.0f - r);
		points.push_back(center + r* Vec2::forAngle(M_PI - M_PI_2 * i / (float)_segment));
	}

	_drawNode->drawSolidPoly(points.data(), points.size(), _backgroudColor);
}
