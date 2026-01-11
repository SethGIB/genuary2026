#include "genuary202610App.h"

void genuary202610App::setup()
{
}

void genuary202610App::mouseDown( MouseEvent event )
{
}

void genuary202610App::update()
{
}

void genuary202610App::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}

void prepareSettings(App::Settings* settings)
{
	settings->setWindowSize(540, 960);
}

CINDER_APP( genuary202610App, RendererGl, prepareSettings )
