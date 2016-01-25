#include "MyTime.h"

static const int TIME_RATIO = 100;
static const int SEC_OF_DAY = 3600 * 24;

MyTime* MyTime::getInstance() {
	static MyTime* time = new MyTime();
	return time;
}

void MyTime::addRealMsec(int msec) {
	_realMsec += msec;
}

void MyTime::setRealMsec(int msec) {
	_realMsec = msec;
}

int MyTime::getVirtualDay() {
	return toRealSec() * TIME_RATIO / SEC_OF_DAY;
}

int MyTime::getVirtualHour() {
	return (toRealSec() * TIME_RATIO - getVirtualDay()*SEC_OF_DAY) / 3600 ;
}
