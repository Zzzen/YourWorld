#ifndef __PSEUDORANDOM_NUMBER_H__
#define __PSEUDORANDOM_NUMBER_H__
#include <vector>

class PseudorandomNumber{
public:
	static PseudorandomNumber* getInstance();
	int getNumber(int index);
private:
	PseudorandomNumber();
	std::vector<int> _randomNumbers;
	void initRandomNumbers(size_t size = 1000);
};


#endif