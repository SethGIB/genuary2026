#pragma once
#include "cinder/gl/gl.h"

using namespace ci;
using namespace std;

struct ffParticle
{
public:
	vec2 mPos;
	vec2 mVel;

	ffParticle() : mPos(vec2(0.0f)), mVel(vec2(0.0f)) {}
	ffParticle(vec2 pos, vec2 vel) : mPos(pos), mVel(vel) {}
};

class ffEmitter
{
public:
	vector<ffParticle> mParticles;
	int mCount;
	ColorA mColor;
	vec2 mOrigin;

	ffEmitter();
	ffEmitter(int ffCount, ColorA ffColor);
	void step();
	void show();
};