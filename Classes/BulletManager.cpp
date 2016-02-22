#include "BulletManager.h"
#include "SpriteManager.h"

#include "Jian.h"

static BulletManager* _instance = nullptr;

BulletManager * BulletManager::getInstance()
{
	if (!_instance) {
		_instance = new BulletManager();
		return _instance;
	}
	else {
		return _instance;
	}
}

void BulletManager::addBullet(Bullet * bullet)
{
	_bullets.pushBack(bullet);
}

void BulletManager::removeBullet(Bullet * bullet)
{
	auto it = _bullets.find(bullet);
	if (it != _bullets.end()) {
		_bullets.erase(it);
	}
}

void BulletManager::updateBullets()
{
	auto vas = SpriteManager::getInstance()->getVas();

	for (auto as : vas) { as->retain(); }

	for (auto it = _bullets.begin(); it != _bullets.end();) {
		if ((*it)->isDone()) {
			assert((*it)->getParent() != nullptr);

			(*it)->removeFromParent();
			it = _bullets.erase(it);
		}
		else {
			bool collided = false;
			for (auto as : vas) {
				if ((*it)->isCollided(as)) {
					(*it)->explode();

					assert((*it)->getParent() != nullptr);

					(*it)->removeFromParent();
					it = _bullets.erase(it);
					collided = true;
					break;
				}
			}
			if (!collided) {
				it++;
			}
		}
	}

	for (auto as : vas) { as->release(); }
}
