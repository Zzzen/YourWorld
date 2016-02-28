#include "CommonDefinition.h"

const PhysicsBodyConfig PhysicsBodyConfig::BULLET = {
	PhysicsBodyConfig::Tag::BULLET_TAG,
	0xFFFF0000,
	0x00000000,
	0x0000FF00
};

const PhysicsBodyConfig PhysicsBodyConfig::MOB = {
	PhysicsBodyConfig::Tag::MOB_TAG,
	0x0000FF00,
	0x00000000,
	0x00FF0000
};

const PhysicsBodyConfig PhysicsBodyConfig::YOU = {
	PhysicsBodyConfig::Tag::YOU_TAG,
	0x0000FF00,
	0x00000000,
	0x00FF0000
};

const PhysicsBodyConfig PhysicsBodyConfig::CROP = {
	PhysicsBodyConfig::Tag::CROP_TAG,
	0x00000000,
	0x00000000,
	0x00000000
};