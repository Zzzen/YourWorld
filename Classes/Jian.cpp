#include "Jian.h"
#include "AttackableSprite.h"
#include "SpriteManager.h"
#include "Utils.h"

Jian * Jian::create(float direction, float velocity, float duration, float damage, AttackableSprite * source)
{
	auto j = new Jian(duration, Vec2::forAngle(direction) * duration * velocity, source);

	j->setRotation(-MATH_RAD_TO_DEG(direction));

	if (j->init()) {
		return j;
	}
	else {
		CC_SAFE_DELETE(j);
		return nullptr;
	}
}

bool Jian::isCollided(AttackableSprite * sprite) const
{
	return getBoundingBox().intersectsRect(sprite->getBoundingBox());
}

Jian::Jian(float duration, const Vec2 & deltaVec, AttackableSprite * source):
	Bullet(duration, deltaVec, source),
	_damage(10.0f)
{
}

void Jian::updateCustom(float dt) {
	auto sprites = SpriteManager::getInstance()->getAllSprites();
	for (auto sp : sprites) {
		auto att = dynamic_cast<AttackableSprite*>(sp);
		if (att && isCollided(att) && att!=_source) {
			Xu x(this);
			removeFromParent();

			att->setHP(att->getHP() - 5);
			return;
		}
	}
}

bool Jian::init()
{
	if (!initWithFile("Jian.png")) return false;

	schedule(CC_SCHEDULE_SELECTOR(Jian::updateCustom), 0.1f);

	return true;
}

void Jian::explode()
{
}
