#include "Jian.h"
#include "AttackableSprite.h"
#include "SpriteManager.h"
#include "Utils.h"
#include "DamageEvent.h"


Jian * Jian::create(float direction, float velocity, float duration, float damage, AttackableSprite * source)
{
	auto j = new Jian(duration, Vec2::forAngle(direction) * duration * velocity, source);

	j->setRotation(-MATH_RAD_TO_DEG(direction));

	if (j->init()) {
		j->autorelease();
		return j;
	}
	else {
		CC_SAFE_DELETE(j);
		return nullptr;
	}
}
//
//bool Jian::isCollided(AttackableSprite * sprite) const
//{
//	return getBoundingBox().intersectsRect(sprite->getBoundingBox()) && _source != sprite ;
//}

Jian::Jian(float duration, const Vec2 & deltaVec, AttackableSprite * source):
	Bullet(duration, deltaVec, source),
	_damage(10.0f)
{
}

bool Jian::init()
{
	if (!initWithFile("jian.png")) {
		return false;
	}

	initPhysicsBody();
	return true;
}

void Jian::initPhysicsBody()
{
	auto body = PhysicsBody::createBox(getContentSize());
	body->setGroup(-2);

	body->setCategoryBitmask(PhysicsBodyConfig::BULLET.categoryBitmask);
	body->setCollisionBitmask(PhysicsBodyConfig::BULLET.collisionBitmask);
	body->setContactTestBitmask(PhysicsBodyConfig::BULLET.contactTestBitmask);

	setPhysicsBody(body);

	auto contactListener = EventListenerPhysicsContactWithGroup::create(-2);
	contactListener->onContactBegin = [this](PhysicsContact& contact) {
		auto nodeA = contact.getShapeA()->getBody()->getNode();
		auto nodeB = contact.getShapeB()->getBody()->getNode();

		if ( (nodeA == this && nodeB != _source)
			|| (nodeB == this && nodeA != _source)) {
			explode();
		
			//if (nodeA != this) {
			//	nodeA->getPhysicsBody()->applyImpulse(nodeA->getPhysicsBody()->getMass()*Vec2(10, 10));
			//}
			//else {
			//	nodeB->getPhysicsBody()->applyImpulse(nodeB->getPhysicsBody()->getMass()*Vec2(10, 10));
			//}

			//retain();
			removeFromParent();
			//scheduleOnce([this](float dt) { release(); }, 0.f, "onContact");
		}

		return false;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
}

void Jian::explode()
{
	auto aabb = getBoundingBox();
	aabb.size = aabb.size * 1.3f;
	auto event = DamageEvent::create(getPosition(), aabb, 5.0f, _source);

	_eventDispatcher->dispatchCustomEvent(DamageEvent::getEventName(), event);

}
