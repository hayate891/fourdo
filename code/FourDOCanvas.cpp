#include "FourDOCanvas.hpp"

const int bitmapWidth    = 320;
const int bitmapHeight   = 240;
const int bitmapBitDepth = 24;

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
	
	// Initialize bitmap.
	int bitmapSize = bitmapWidth * bitmapHeight * (bitmapBitDepth / 8);
	mBitmapBytes = new GLubyte[ bitmapSize ];
	memset( mBitmapBytes, 0, bitmapSize );
	
	this->UpdateImage();
}
FourDOCanvas::~FourDOCanvas()
{
	delete[] mBitmapBytes;
}

uchar* FourDOCanvas::getVRAMPointer()
{
	return mVRAMPointer;
}

void FourDOCanvas::setVRAMPointer( uchar* vramPointer )
{
	mVRAMPointer = vramPointer;
}

void FourDOCanvas::UpdateImage()
{
	int x = 0;
	int y = 0;
	int byteNum;

	uchar r;
	uchar g;
	uchar b;

	for (int i = 0; i < bitmapHeight * bitmapWidth * 2; i+= 2)
	{
		// 0RRRRRGGGGGBBBBB
		r = (mVRAMPointer[i] >> 2) & 0x1F;
		g = ((mVRAMPointer[i] & 0x03) << 3) | (mVRAMPointer[i + 1] >> 5);
		b = mVRAMPointer[i + 1] & 0x1F;

		byteNum = (y*bitmapWidth + x)*(bitmapBitDepth/8);
		mBitmapBytes[byteNum]=r*8;
		mBitmapBytes[++byteNum]=g*8;
		mBitmapBytes[++byteNum]=b*8;

		if (i % 4)
		{
			++x;
			--y;
		}
		else
		{
			++y;
		}
		
		if (x == bitmapWidth)
		{
			// Move two lines down.
			x =  0;
			y += 2;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////
// Protected/Private

void FourDOCanvas::OnPaint( wxPaintEvent& )
{
	wxPaintDC(this);
	
	this->Render();
	//Refresh(false);
}

void FourDOCanvas::OnErase( wxEraseEvent& WXUNUSED(event) )
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
	
	// Clear everything
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	/////////////////////
	// Draw some crap.
	glPixelZoom((float)width/bitmapWidth, -(float)height/bitmapHeight);
	glRasterPos2i(0, 0);
	glDrawPixels( 
		bitmapWidth,
		bitmapHeight,
        GL_RGB, 
        GL_UNSIGNED_BYTE, 
        mBitmapBytes);

    //glFlush();
    SwapBuffers();
}

void FourDOCanvas::InitGL()
{
    SetCurrent();

    /* set viewing projection */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity (); // Load identity matrix
	glOrtho (0, 320, 240, 0, 0, 1);
	glDisable(GL_DEPTH_TEST);

    /* position viewer */
    glMatrixMode(GL_MODELVIEW);

	// Displacement trick for exact pixelization
	//glTranslatef(0.375, 0.375, 0)
}