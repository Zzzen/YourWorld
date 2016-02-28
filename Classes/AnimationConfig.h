#pragma once

#include <string>

struct AnimationConfig{
    std::string fileName;
    int idleEndFrame;
    int attackStartFrame;
    int attackEndFrame;
    int freezedStartFrame;
    int freezedEndFrame;

    static const AnimationConfig yourSkeleton;
	static const AnimationConfig cao;
	static const AnimationConfig liZhi;
};
