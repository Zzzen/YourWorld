#include "Equipment.h"
#include "i18n.h"

class Dabaojian : public Equipment {
public:
	static Dabaojian* create();

	void attach(AttackableSprite* owner) override;
	void detach(AttackableSprite* owner) override;

	const string getClassName() const override { return "Dabaojian"; }
	virtual string getDetails() const override { return R::getString(R::DABAOJIAN_DETAILS); };
protected:
	static const float STRENGTH;
};