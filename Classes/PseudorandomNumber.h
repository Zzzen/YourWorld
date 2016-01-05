#ifndef __PSEUDORANDOM_NUMBER_H__
#define __PSEUDORANDOM_NUMBER_H__
#include <vector>
#include <random>

using namespace std;

class PseudorandomNumber{
public:
	static PseudorandomNumber* getInstance();
	float getNumber(size_t index);
private:
	PseudorandomNumber();
	vector<float> _randomNumbers;
	void initRandomNumbers(size_t size = 1000);
	minstd_rand _gen;
	uniform_real_distribution<> _dis;
};


#endif