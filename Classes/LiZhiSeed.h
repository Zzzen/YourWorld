#pragma once

#include "Seed.h"

class Crop;

class LiZhiSeed : public Seed {
public:
	CREATE_FUNC(LiZhiSeed);

	static LiZhiSeed* createWithJson(const Document& json);

	Crop* createCrop() override;

	bool init() override;

	string getClassName()const override { return "LiZhiSeed"; }

	string getDetails()const override { return "LIZHISEED"; }
};