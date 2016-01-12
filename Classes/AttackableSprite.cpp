#include "AttackableSprite.h"

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
		//play attack animation
		break;
	case AttackableSprite::FLEE:
		break;
	case AttackableSprite::FREEZED:
		
		break;
	default:
		break;
	}
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
	const string follow = string(nodeName).insert(dot, "Follow");
	const string attack = string(nodeName).insert(dot, "Attack");
	const string flee = string(nodeName).insert(dot, "Flee");
	const string freezed = string(nodeName).insert(dot, "Freezed");

	auto followAction = CSLoader::createTimeline(follow);
	auto attackAction = CSLoader::createTimeline(attack);
	auto fleeAction = CSLoader::createTimeline(flee);
	auto freezedAction = CSLoader::createTimeline(freezed);

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

	return true;
}
