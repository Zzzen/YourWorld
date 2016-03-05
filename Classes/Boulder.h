#pragma once

#include "Mineral.h"

USING_NS_CC;

class Boulder : public Mineral {
public:
	static Boulder* create();
	
	CREATE_WITH_JSON_FUNC(Boulder);

	Document toJson() const override;

	bool initWithJson(const Document& json);

	void onMined(const MiningEventData& data) override;

	virtual void setProgress(int progress);

	virtual int getProgress()const { return _progress; }

	Boulder();

	string getClassName() const { return "Boulder"; }

protected:
	int _progress;
};