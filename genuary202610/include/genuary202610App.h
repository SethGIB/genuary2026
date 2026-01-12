#pragma once
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Json.h"
#include "cinder/Camera.h"
#include "cinder/CameraUi.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class genuary202610App : public App {
public:
	void setup() override;
	void mouseDown(MouseEvent event) override;
	void update() override;
	void draw() override;

private:
	Json mSphereJson;
	CameraPersp mCamera;
	CameraUi mNavCtrl;

	gl::BatchRef mInstancedObjects;

	void setupInstances();

	void setupCamera();
};