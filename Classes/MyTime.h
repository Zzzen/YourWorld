#pragma once

class MyTime {
public:
	static MyTime* getInstance();

	void addRealMsec(int msec);
	void setRealMsec(int msec);

	int getVirtualHour();
	int getVirtualDay();

	int toRealSec() const { return _realMsec / 1000; }
private:
	int _realMsec;
};