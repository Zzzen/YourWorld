#include "AttackableSprite.h"
#include "DamageEvent.h"
#include "Utils.h"
#include "Equipment.h"
#include "Consumable.h"

Document AttackableSprite::toJson() const
{
	auto json = LivingSprite::toJson();

	json.AddMember("state", rapidjson::Value(_state), json.GetAllocator());

	return json;
}

void AttackableSprite::setCurrentState(SpriteState state)
{
	auto lastStateAction = _actions[_state];
	_skeletalNode->stopAction(lastStateAction);

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
	Xu xu(this);
	auto damage = static_cast<DamageEvent*> (event->getUserData());
	CCASSERT(damage, "Something is wrong with damage event");

	if ( damage->getSource()==this || !getBoundingBox().intersectsRect(damage->getRange()) ) {
		return;
	}
	setCurrentState(FREEZED);
	auto value = damage->getDamage() - getArmor();
	value = value < 0 ? 1 : value;
	setHP(getHP() - value);
//	CCLOG("%p onAttacked; damage: %f", this, value);
}

void AttackableSprite::setPosition(const Point& position)
{
	auto oldPosition = getPosition();
	LivingSprite::setPosition(position);

	float flip = position.x > oldPosition.x ? -1 : 1;
	_skeletalNode->setScaleX(flip);

	_direction = (position - oldPosition).getNormalized();
}

void AttackableSprite::setHP(int hp)
{
	LivingSprite::setHP(hp);
	_HPBar->setPercentage(100 * getHP() / getMaxHP());
}

void AttackableSprite::equip(Equipment* equip) {
	//remove original equipment
	auto it = _equipments.find(equip->_equipmentType);
	if (it != _equipments.end()) {
		it->second->detach(this);
		it->second->release();
	}

	_equipments[equip->_equipmentType] = equip;
	equip->attach(this);
	equip->retain();
}

void AttackableSprite::consume(Consumable * consumable)
{
	consumable->beConsumed(this);
	_inventory.eraseObject(consumable);
}

Vector<StatusEffect*> AttackableSprite::getStatusEffects()
{
	Vector<StatusEffect*> effs;
	for (auto child : getChildren()) {
		auto eff = dynamic_cast<StatusEffect*>(child);
		if (eff) effs.pushBack(eff);
	}
	return effs;
}

void AttackableSprite::addStatusEffect(StatusEffect * effect)
{
	//if this kind of effect has been added.
	auto name = effect->getClassName();
	for (auto eff : getStatusEffects()) {
		if (eff->getClassName() == name) {
			eff->refresh();
			return;
		}
	}

	addChild(effect);
	effect->setTarget(this);
}

void AttackableSprite::removeStatusEffect(StatusEffect * effect)
{
	removeChild(effect, true);
}

AttackableSprite::~AttackableSprite()
{
	for (auto it = _actions.begin(); it != _actions.end(); ) {
		it->second->release();
		it = _actions.erase(it);
	}
	for (auto it = _equipments.begin(); it != _equipments.end();) {
		it->second->release();
		it = _equipments.erase(it);
	}
}

bool AttackableSprite::init()
{
	auto HPPic = Sprite::create("HP.png");
	HPPic->setVisible(false);

	_HPBar = ProgressTimer::create(HPPic);
	_HPBar->addChild(HPPic);
	_HPBar->setType(ProgressTimer::Type::RADIAL);
	addChild(_HPBar);
	_HPBar->setPercentage(100);

	_originalStrength = getOriginalStrength();
	_orginalArmor = getOriginalArmor();
	_originalMoveSpeed = getOriginalMoveSpeed();

	if (!LivingSprite::init()) {
		return false;
	}

	_HPBar->setPosition(Vec2(0, 40));

	initActions();

	setCurrentState(IDLE);

	auto damageListener = EventListenerCustom::create(DamageEvent::getEventName(),
		CC_CALLBACK_1(AttackableSprite::onAttacked, this));

	_eventDispatcher->addEventListenerWithSceneGraphPriority(damageListener, this);

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

AttackableSprite::AttackableSprite():
	_state(IDLE),
	_extraArmor(0.0f),
	_extraMoveSpeed(0.0f),
	_extraStrength(0.0f),
	_originalMoveSpeed(0.0f),
	_originalStrength(0.0f),
	_orginalArmor(0.0f)
{}