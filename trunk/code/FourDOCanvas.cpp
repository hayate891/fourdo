#include "FourDOCanvas.hpp"

FourDOCanvas::FourDOCanvas
		(
		uchar* vramPointer,
		wxWindow *parent, 
		wxWindowID id
		)
	: wxGLCanvas
		(
		parent, 
		(wxGLCanvas*)NULL,
		id, 
		wxDefaultPosition, 
		wxDefaultSize, 
		wxFULL_REPAINT_ON_RESIZE, 
		wxGLCanvasName
		)
	, mVRAMPointer( vramPointer )
	, mInitialized( false )
{
	Connect( wxEVT_PAINT,		     wxPaintEventHandler(FourDOCanvas::OnPaint ) );
	Connect( wxEVT_SIZE,             wxSizeEventHandler (FourDOCanvas::OnSize  ) );
	Connect( wxEVT_ERASE_BACKGROUND, wxEraseEventHandler(FourDOCanvas::OnErase ) );
}
FourDOCanvas::~FourDOCanvas()
{
}

uchar* FourDOCanvas::getVRAMPointer()
{
	return mVRAMPointer;
}

void FourDOCanvas::setVRAMPointer( uchar* vramPointer )
{
	mVRAMPointer = vramPointer;
}

////////////////////////////////////////////////////////////////////////////////////

void FourDOCanvas::OnPaint( wxPaintEvent& )
{
	this->Render();
	//Refresh(false);
}

void FourDOCanvas::OnErase( wxEraseEvent& event)
{
	// Do nothing, to avoid flashing.
}

void FourDOCanvas::OnSize( wxSizeEvent& event)
{
	// Call base first!
	wxGLCanvas::OnSize(event);
}

void FourDOCanvas::Render()
{
    wxPaintDC(this);

	#ifndef __WXMOTIF__
    if( !GetContext() )
		return;
	#endif
    
    // Init OpenGL once, but after SetCurrent
    if (!mInitialized)
    {
        InitGL();
        mInitialized = true;
    }

	/////////////////////
	// Set the viewport size equal to window size.
	int width;
	int height;
	GetClientSize(&width, &height);	
	SetCurrent();
	glViewport(0, 0, (GLint)width, (GLint)height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-0.5f, 0.5f, -0.5f, 0.5f, 1.0f, 3.0f);
    glMatrixMode(GL_MODELVIEW);

    // clear color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	/////////////////////
	// Draw some crap.

    glBegin(GL_QUADS);
    glNormal3f( 0.0f, 0.0f, 1.0f);
    glVertex3f( 0.5f, 0.5f, 0.5f); glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f,-0.5f, 0.5f); glVertex3f( 0.5f,-0.5f, 0.5f);

    glNormal3f( 0.0f, 0.0f,-1.0f);
    glVertex3f(-0.5f,-0.5f,-0.5f); glVertex3f(-0.5f, 0.5f,-0.5f);
    glVertex3f( 0.5f, 0.5f,-0.5f); glVertex3f( 0.5f,-0.5f,-0.5f);

    glNormal3f( 0.0f, 1.0f, 0.0f);
    glVertex3f( 0.5f, 0.5f, 0.5f); glVertex3f( 0.5f, 0.5f,-0.5f);
    glVertex3f(-0.5f, 0.5f,-0.5f); glVertex3f(-0.5f, 0.5f, 0.5f);

    glNormal3f( 0.0f,-1.0f, 0.0f);
    glVertex3f(-0.5f,-0.5f,-0.5f); glVertex3f( 0.5f,-0.5f,-0.5f);
    glVertex3f( 0.5f,-0.5f, 0.5f); glVertex3f(-0.5f,-0.5f, 0.5f);

    glNormal3f( 1.0f, 0.0f, 0.0f);
    glVertex3f( 0.5f, 0.5f, 0.5f); glVertex3f( 0.5f,-0.5f, 0.5f);
    glVertex3f( 0.5f,-0.5f,-0.5f); glVertex3f( 0.5f, 0.5f,-0.5f);

    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(-0.5f,-0.5f,-0.5f); glVertex3f(-0.5f,-0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f); glVertex3f(-0.5f, 0.5f,-0.5f);
    glEnd();

    glFlush();
    SwapBuffers();
}

void FourDOCanvas::InitGL()
{
    SetCurrent();

    /* set viewing projection */
    glMatrixMode(GL_PROJECTION);
    glFrustum(-0.5f, 0.5f, -0.5f, 0.5f, 1.0f, 3.0f);

    /* position viewer */
    glMatrixMode(GL_MODELVIEW);
    glTranslatef(0.0f, 0.0f, -2.0f);

    /* position object */
    glRotatef(30.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(30.0f, 0.0f, 1.0f, 0.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}
