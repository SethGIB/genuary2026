#include "genuary202602App.h"

void genuary202602App::setup()
{
}

void genuary202602App::mouseDown( MouseEvent event )
{
}

void genuary202602App::update()
{
}

void genuary202602App::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}

void prepareSettings( App::Settings* settings ) {
	settings->setWindowSize(540, 960);
}
CINDER_APP( genuary202602App, RendererGl, *prepareSettings )
