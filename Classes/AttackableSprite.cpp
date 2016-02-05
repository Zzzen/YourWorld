#include "AttackableSprite.h"
#include "DamageEvent.h"
#include "Utils.h"
#include "Equipment.h"
#include "Consumable.h"
#include "CommonDefinition.h"

static const int STATE_ACTION_TAG = 23421;

Document AttackableSprite::toJson() const
{
	auto json = LivingSprite::toJson();

	json.AddMember("state", rapidjson::Value(_state), json.GetAllocator());

	return json;
}

void AttackableSprite::setCurrentState(SpriteState state)
{
	_state = state;

	switch (state)
	{
	case AttackableSprite::IDLE:
		//walk around
		_idleAction->gotoFrameAndPlay(0, 21, true);
		break;
	case AttackableSprite::FOLLOW:
		//run towards player
		_idleAction->gotoFrameAndPlay(0, 21, true);
		break;
	case AttackableSprite::ATTACK:
		//play attackFile animation
		_idleAction->gotoFrameAndPlay(30, 51, true);
		break;
	case AttackableSprite::FLEE:
		_idleAction->gotoFrameAndPlay(0, 21, true);
		break;
	case AttackableSprite::FREEZED:
		_idleAction->gotoFrameAndPlay(60, 81, true);
		break;
	default:
		CC_ASSERT(false);
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

	//show a damage label
	auto label = Label::createWithSystemFont(to_string((int)(damage->getDamage())), DEFUALT_FONT, 16);
	label->setTextColor(Color4B::RED);
	label->setPosition(0, getBoundingBox().size.height);
	auto moveBy = MoveBy::create(1.0f, Vec2(0, 20));
	label->runAction(moveBy);
	auto scale = ScaleTo::create(moveBy->getDuration(), 1.4f);
	label->runAction(scale);
	scheduleOnce([label](float dt) {  label->removeFromParent();  }, moveBy->getDuration(), to_string((long long)label));
	addChild(label);
//	CCLOG("%p onAttacked; damage: %f", this, value);
}

void AttackableSprite::setPosition(float x, float y)
{
	Point position(x, y);
	auto prevPosition = getPosition();
	LivingSprite::setPosition(x, y);

	_direction = (position - prevPosition).getNormalized();

	if (abs(position.x - prevPosition.x) < std::numeric_limits<float>::epsilon()) return;
	float flip = position.x > prevPosition.x ? -1 : 1;
	_skeletalNode->setScaleX(flip);
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
		[this](EventCustom * event) { onAttacked(event); });

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

	return true;
}

bool AttackableSprite::initActions()
{
	using namespace cocostudio::timeline;

	_idleAction->setTag(STATE_ACTION_TAG);
	_idleAction->setFrameEventCallFunc([this](Frame* frame) {
		EventFrame* evnt = dynamic_cast<EventFrame*>(frame);
		if (!evnt || evnt->getEvent().empty())	return;
		string evtName = evnt->getEvent();
		//CCLOG("index: %d : %s", evnt->getFrameIndex() , evtName.c_str());
		if (evtName.find("_END") != std::string::npos) {
			setCurrentState(IDLE);
		}
		else if ("ATTACK"==evtName)
		{
			attack();
		}
		else
		{
			//CCLOG("no action found for: %s", evtName.c_str());
		}
	});

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