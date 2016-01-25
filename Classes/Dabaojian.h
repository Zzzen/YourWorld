#include "Equipment.h"


class Dabaojian : public Equipment {
public:
	static Dabaojian* create();

	void attach(AttackableSprite* owner) override;
	void detach(AttackableSprite* owner) override;

	const string getClassName() const override { return "Dabaojian"; }

protected:
	static const float STRENGTH;
};