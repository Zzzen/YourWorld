#pragma once


template <typename SpriteType>
class SpriteEventData: public Ref {
public:
	inline SpriteType* getWho() { return _who; }
	
	virtual ~SpriteEventData() { if (_who) { _who->release(); } }

protected:
	SpriteType* _who;

	bool initWithWho(SpriteType* const who) {
		_who = who;
		_who->retain();
		return true;
	}

	SpriteEventData(): _who(nullptr) {}
};
