#include "PseudorandomNumber.h"
#include "cocos2d.h"

USING_NS_CC;

static PseudorandomNumber* _instance = nullptr;

PseudorandomNumber* PseudorandomNumber::getInstance(){
	if (!_instance){
		_instance = new PseudorandomNumber();
	}

	return _instance;
}

PseudorandomNumber::PseudorandomNumber(){
	initRandomNumbers();
}

inline int lcg(int seed){
	return seed * 48271 % 0xffffffff;
}

int PseudorandomNumber::getNumber(int index){
	assert(index < _randomNumbers.size());
	return _randomNumbers[index];
}

void PseudorandomNumber::initRandomNumbers(size_t size){
	int seed = 1234;
	for (size_t i = 0; i < size; i++){
		seed = lcg(seed);
		_randomNumbers.push_back(seed);
		//CCLOG("%u", seed);
		if (_randomNumbers.size() % 10000 == 0){
			CCLOG("%u", _randomNumbers.size());
		}
	}
}