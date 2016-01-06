#ifndef __STATUE_H__
#define __STATUE_H__

#include "SerializableSprite.h"
#include "SpriteEvent.h"


class Statue : public SerializableSprite {
public:
	enum Type
	{
		SPEED = 1,
		HEAL = 2
	};

	CC_SYNTHESIZE(Type, _type, Type);

	//+ statue::type
	Document toJson() const override;
	const std::string getClassName() const override { return "Statue"; }

	static Statue* createWithType(const Type type);

	static Statue* createWithJson(const Document& json);

protected:
	Statue();
	bool initWithJson(const Document& json) override;
	bool initWithType(const Type type);
};

class StatueJoinWorldEvent : SpriteEvent<Statue> {
public:
	static std::string getName() {
		return "statue_join_world_event";
	}
};

class StatueDisjoinWorldEvent : SpriteEvent<Statue> {
public:
	static std::string getName() {
		return "statue_disjoin_world_event";
	}
};

#endif // !__STATUE_H__
