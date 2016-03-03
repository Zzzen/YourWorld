#ifndef __SPRITE_EVENT_H__
#define __SPRITE_EVENT_H__


class You;
class Chunk;
template <typename SpriteType>
class SpriteEvent: public Ref {
public:
	inline SpriteType* getWho() { return _who; }
	
	virtual ~SpriteEvent() { _who->release(); }

protected:
	SpriteType* _who;

	bool initWithWho(SpriteType* const who) {
		_who = who;
		_who->retain();
		return true;
	}

	SpriteEvent(): _who(nullptr) {}
};



#endif // !__SPRITE_EVENT_H__
