#include "genuary202602App.h"
#include "cinder/Rand.h"
#include "cinder/CinderMath.h"

// Main App //////////////////////////////////////////////////////////////////////////////
int kCount = 5000;
void genuary202602App::setup()
{
	auto g = vec2(0.0f, 0.5f);
	mArcSystem = ArcSystem(kCount, g);
	gl::clear(Color(0.0f, 0.0f, 0.0f));

	//gl::enableAlphaBlending();
}

void genuary202602App::mouseDown( MouseEvent event )
{
	mIsRunning = !mIsRunning;
}

void genuary202602App::update()
{
	if(mIsRunning)
		mArcSystem.step();
}

void genuary202602App::draw()
{
	gl::enableAdditiveBlending();
	gl::color(ColorA(0.25f, 0.6f, 0.9f, 0.25f));
	mArcSystem.show();
	gl::disableBlending();

	gl::enableAlphaBlending();
	gl::color(ColorA(0.0f, 0.0f, 0.0f, 0.05f));
	gl::drawSolidRect(Rectf(0, 0, getWindowWidth(), getWindowHeight()));
	gl::disableAlphaBlending();
}

void prepareSettings( App::Settings* settings ) {
	settings->setWindowSize(540, 960);
}
CINDER_APP( genuary202602App, RendererGl, *prepareSettings )


