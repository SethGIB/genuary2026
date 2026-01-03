#pragma once
#include "cinder/gl/gl.h"

using namespace ci;
using namespace std;

struct ffParticle
{
public:
	vec3 mPos;
	vec3 mVel;
};

class ffEmitter
{
public:
	vector<ffParticle> mParticles;
	int mCount;
	int mAge = 15;
	vec4 mColor;
	vec3 mOrigin;

	ffEmitter();
	ffEmitter(int ffCount, vec4 ffColor, vec3 ffOrigin);
	void step();
	void show();
};