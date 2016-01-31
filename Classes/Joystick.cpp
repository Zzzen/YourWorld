#include "Joystick.h"
#include "Utils.h"

static const float PI = 3.1415926f;

Vec2 Joystick::getGradientVector()
{
	if (!_arrow->isVisible() || _arrow->getScale() < std::numeric_limits<float>::epsilon()) {
		return Vec2::ZERO;
	}

	return Vec2::forAngle( MATH_DEG_TO_RAD( - (_arrow->getRotation() - 90.0f) ) ).getNormalized();
}

bool Joystick::isPointAccepted(const Point & pos)
{
	return !FreeArrow::isPointAccepted(pos);   //to do
}

bool Joystick::init()
{
	if (!FreeArrow::init()) return false;

	setCircleColor(Color4F::MAGENTA);

	return true;
}
