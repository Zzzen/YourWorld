#include "PseudorandomNumber.h"
#include "cocos2d.h"

USING_NS_CC;


PseudorandomNumber* PseudorandomNumber::getInstance(){
	static auto instance = new PseudorandomNumber();

	return instance;
}

PseudorandomNumber::PseudorandomNumber()
	:_gen(110)
	,_dis(0, M_PI)
{
	initRandomNumbers();
}

float PseudorandomNumber::getNumber(size_t index){
	assert(index < _randomNumbers.size());
	return _randomNumbers[index];
}

void PseudorandomNumber::initRandomNumbers(size_t size){
	for (size_t i = 0; i < size; i++){
		_randomNumbers.push_back(_dis(_gen));
		if (_randomNumbers.size() % 10000 == 0){
			CCLOG("%u", _randomNumbers.size());
		}
	}
}