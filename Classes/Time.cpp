#include "Time.h"

static const int TIME_RATIO = 100;
static const int SEC_OF_DAY = 3600 * 24;

Time* Time::getInstance() {
	static Time* time = new Time();
	return time;
}

void Time::addRealMsec(int msec) {
	_realMsec += msec;
}

void Time::setRealMsec(int msec) {
	_realMsec = msec;
}

int Time::getVirtualDay() {
	return toRealSec() * TIME_RATIO / SEC_OF_DAY;
}

int Time::getVirtualHour() {
	return (toRealSec() * TIME_RATIO - getVirtualDay()*SEC_OF_DAY) / 3600 ;
}
