#pragma once

#include "Bullet.h"

class BulletManager {
public:
	static BulletManager* getInstance();

	void addBullet(Bullet* bullet);
	void removeBullet(Bullet* bullet);

	void updateBullets();

private:
	Vector<Bullet*> _bullets;

};