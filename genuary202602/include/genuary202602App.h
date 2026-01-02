#pragma once
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "ArcSystemWorks.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class genuary202602App : public App {
public:
	void setup() override;
	void mouseDown(MouseEvent event) override;
	void update() override;
	void draw() override;

private:
	bool mIsRunning = false;
	ArcSystem mArcSystem;
};
