#pragma once
#include "cinder/gl/gl.h"

using namespace ci;
using namespace std;

class ArcSystem; // Forward declaration	

class ArcLite {
	vec2 mPos;
	vec2 mSpd;
	float mAge;
	float mLife;
	float mMass;
	float mRadius;

public:
	bool isDead() { return mAge <= 0.0f; }
	ArcLite();

	void step(vec2 _gravity);
	void show();
	void reset();

};

class ArcSystem {
public:
	int mCount = 0;
	vector<ArcLite> mArcList;
	vec2 mGravity;

	void step();
	void show();
	vec2 getGravity() { return mGravity; }
	ArcSystem();
	ArcSystem(int _count, vec2 _gravity);
};