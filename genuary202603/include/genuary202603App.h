#pragma once
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Camera.h"
#include "cinder/CameraUi.h"
#include "ffEmitter.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class genuary202603App : public App {
public:
	void setup() override;
	void update() override;
	void draw() override;

private:
	int mLastNumber = 0;
	int mNextNumber = 0;

	const int kNumRows = 80;
	const int kNumCols = 120;
	const float kNoiseMin = 0.0f;
	const float kNoiseMax = 0.25f;
	const float kXScale = 0.015f;
	const float kYScale = 0.02f;	
	const float kXStep = 0.1f;
	const float kYStep = 0.08f;

	const vec4 kBlue = vec4(38.0f / 255.0f, 104.0f / 255.0f, 120.0f / 255.0f, 0.9f);
	const vec4 kGreen = vec4(42.0f / 255.0f, 120.0f / 255.0f, 38.0f / 255.0f, 0.2f);
	const vec4 kBrown = vec4(94.0f / 255.0f, 74.0f / 255.0f, 34.0f / 255.0f, 1.0f);
	const vec4 kSky = vec4(64.0f/255.0f, 107.0f / 255.0f, 210.0f / 255.0f, 1.0f);
	const vec4 kSnow = vec4(0.9f, 0.9f, 1.0f, 0.85f);

	const int kFibboA[6] = { 3,3,6,9,15,24 };
	const int kFibboB[6] = { 1,2,3,5,8,13 };

	//Points
	gl::BatchRef mTerrainBatch;
	gl::BatchRef mFireworksBatch;
	gl::VboRef mTerrainBuffer;
	gl::VboRef mFireworksBuffer;

	struct mPoint {
		vec3 position;
		vec4 color;
	};

	vector<mPoint> mTerrainData;
	
	vector<ffEmitter> mFireworksEmitters;
	vector<mPoint> mFireworksData;

	gl::TextureRef mTexBG;

	CameraPersp	mCamera;
	CameraUi	mCameraCtrl;

	void setupCamera();
	void setup3DObjects();
	
	void updateTerrain( float gOffset);

	void setupFireworks();
	void updateFireworks();
};