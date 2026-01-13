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


	const int kFibboA[6] = { 3,3,6,9,15,24 };
	const int kFibboB[6] = { 1,2,3,5,8,13 };

	//Points
	gl::BatchRef mTerrainBatch;
	gl::VboRef mTerrainXyzVbo;
	gl::VboRef mTerrainRgbVbo;
	gl::VboMeshRef mTerrainShape;
	gl::GlslProgRef mTerrainShader;

	struct mPoint {
		vec3 position;
		vec4 color;
	};

	vector<mPoint> mTerrainData;
	gl::TextureRef mTexBG;

	CameraPersp	mCamera;
	CameraUi	mCameraCtrl;

	void setupCamera();
	void setupShaders();
	void setupInstancedMesh();
	
	void updateTerrain( float gOffset);

	void setupFireworks();
	void updateFireworks();
};