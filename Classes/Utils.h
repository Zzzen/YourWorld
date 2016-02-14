#pragma once

#include "cocos2d.h"

#include <string>
#include <sstream>


#define RAPIDJSON_HAS_STDSTRING 1
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"

USING_NS_CC;
using namespace std;

void showToast(string text);

template <class RET_TYPE>
inline RET_TYPE strTo(const string& str)
{
	stringstream ss(str);
	RET_TYPE i;
	ss >> i;
	return i;
}

string jsonToString(const rapidjson::Document& doc);

string str(const Point& point);

string str(const Size& size);

string str(const Rect& rect);

void setNodePauseRecursively(Vector<Node*>& nodes, bool paused);

Vec2 randomVector(float radian);

Size getVisibleSize();

class Xu {
public:
	Xu(Ref* ref) : _ref(ref) { _ref->retain(); }
	~Xu() { _ref->release(); }

	Ref* _ref;
};



namespace std
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream ss;
        ss << n ;
        return ss.str() ;
    }
}