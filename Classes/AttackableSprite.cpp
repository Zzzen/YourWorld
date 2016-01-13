#include "AttackableSprite.h"
#include "DamageEvent.h"
#include "Utils.h"


Document AttackableSprite::toJson() const
{
	auto json = LivingSprite::toJson();

	json.AddMember("state", rapidjson::Value(_state), json.GetAllocator());

	return json;
}

void AttackableSprite::setCurrentState(SpriteState state)
{
	_state = state;
	auto action = _actions[state];
	CCASSERT(action, "no corresponding action");
	_skeletalNode->stopAllActions();
	_skeletalNode->runAction(action);
	action->gotoFrameAndPlay(0, true);

	switch (state)
	{
	case AttackableSprite::IDLE:
		//walk around
		break;
	case AttackableSprite::FOLLOW:
		//run towards player
		break;
	case AttackableSprite::ATTACK:
		//play attackFile animation
		break;
	case AttackableSprite::FLEE:
		break;
	case AttackableSprite::FREEZED:
		
		break;
	default:
		break;
	}
}


void AttackableSprite::onAttacked(EventCustom * event)
{
	auto damage = static_cast<DamageEvent*> (event->getUserData());
	CCASSERT(damage, "Something is wrong with damage event");

	if ( damage->getSource()==this || !getBoundingBox().intersectsRect(damage->getRange()) ) {
		return;
	}
	auto value = damage->getDamage();
	setHP(getHP() - value);
	CCLOG("%p onAttacked; damage: %f", this, value);
}

void AttackableSprite::setPosition(const Point& pos)
{
	auto old = getPosition();
	LivingSprite::setPosition(pos);

	_direction = (pos - old).getNormalized();
}

AttackableSprite::~AttackableSprite()
{
	for (auto it = _actions.begin(); it != _actions.end(); ) {
		it->second->release();
		it = _actions.erase(it);
	}
}

bool AttackableSprite::init()
{
	if (!LivingSprite::init()) {
		return false;
	}

	initActions();

	setCurrentState(IDLE);

	_damageListener = EventListenerCustom::create(DamageEvent::getEventName(),
		                                          CC_CALLBACK_1(AttackableSprite::onAttacked, this));

	return true;
}

bool AttackableSprite::initWithJson(const Document & json)
{
	if (!LivingSprite::initWithJson(json)) {
		return false;
	}

	assert(json.HasMember("state") && json["state"].IsInt());

	auto state = static_cast<SpriteState>(json["state"].GetInt());
	setCurrentState(state);

	return false;
}

bool AttackableSprite::initActions()
{
	const string nodeName = getSkeletalFileName();
	const auto dot = nodeName.find_last_of('.');
	const string followFile = string(nodeName).insert(dot, "Follow");
	const string attackFile = string(nodeName).insert(dot, "Attack");
	const string fleeFile = string(nodeName).insert(dot, "Flee");
	const string freezedFile = string(nodeName).insert(dot, "Freezed");

	auto followAction = CSLoader::createTimeline(followFile);
	auto attackAction = CSLoader::createTimeline(attackFile);
	auto fleeAction = CSLoader::createTimeline(fleeFile);
	auto freezedAction = CSLoader::createTimeline(freezedFile);

	CCASSERT( followAction && attackAction && fleeAction && freezedAction,
		(nodeName + " action files not found.").c_str());

	_actions[IDLE] = _idleAction;
	_actions[FOLLOW] = followAction;
	_actions[ATTACK] = attackAction;
	_actions[FLEE] = fleeAction;
	_actions[FREEZED] = freezedAction;

	for (auto pair : _actions) {
		pair.second->retain();
	}


	//set action callbacks.
	auto setIdle = [this] { setCurrentState(IDLE); };
	followAction->setLastFrameCallFunc(setIdle);
	fleeAction->setLastFrameCallFunc(setIdle);
	freezedAction->setLastFrameCallFunc(setIdle);

	auto attackAndIdle = [this, setIdle] {
		setIdle();
		attack();
	};
	attackAction->setLastFrameCallFunc(attackAndIdle);


	return true;
}
