#include "Equipment.h"
#include "i18n.h"

class Dabaojian : public Equipment {
public:
	static Dabaojian* create();
	static Dabaojian* createWithJson(const Document& doc);

	void attach(AttackableSprite* owner) override;
	void detach(AttackableSprite* owner) override;

	AttackableSprite::EquipmentType getEquipmentType() const { return AttackableSprite::EquipmentType::WEAPON; }

	string getClassName() const override { return "Dabaojian"; }
	string getDetails() const override { return R::getString(R::DABAOJIAN_DETAILS); };
protected:
	static const float STRENGTH;
};