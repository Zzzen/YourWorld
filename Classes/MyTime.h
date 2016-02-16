#pragma once

class MyTime {
public:
	static MyTime* getInstance();

	void addRealMsec(int msec);
	void setRealMsec(long long msec);
	long long getRealMesc() const { return _realMsec; }

	int getVirtualHour() const;
	int getVirtualDay() const;

	long long toRealSec() const { return _realMsec / 1000; }
private:
	long long _realMsec;
};
