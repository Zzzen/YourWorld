#ifndef __SPRITE_EVENT_H__
#define __SPRITE_EVENT_H__

template <typename SpriteType>
class SpriteEvent: public Ref {
public:
	static SpriteEvent* createWithWho(SpriteType* const sprite){
		auto event = new SpriteEvent<SpriteType>(sprite);
		event->autorelease();
		return event;
	}

	inline SpriteType* getWho() { return _who; }

protected:
	SpriteType* const _who;

	SpriteEvent(SpriteType* const who) : _who(who) { who->retain(); }
	virtual ~SpriteEvent() { _who->release(); }

};



#endif // !__SPRITE_EVENT_H__
