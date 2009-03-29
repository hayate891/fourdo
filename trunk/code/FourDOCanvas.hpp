#ifndef _INC_FOURDOCANVAS
#define _INC_FOURDOCANVAS

#include "wx/wx.h"
#include "wx/glcanvas.h"

#include "types.h"

class FourDOCanvas : public wxGLCanvas
{
public:
    FourDOCanvas
		(
		uchar* vramPointer,
		wxWindow *parent, 
		wxWindowID id = wxID_ANY
		);
	~FourDOCanvas();
	
	void UpdateImage();

	uchar* getVRAMPointer();
	void   setVRAMPointer(uchar* pointer);
		
protected:
	void OnPaint( wxPaintEvent& event );
	void OnErase( wxEraseEvent& event );
	void OnSize ( wxSizeEvent&  event );
	
	void Render();
	void InitGL();
	
	uchar* mVRAMPointer;
	bool   mInitialized;
	
	GLubyte* mBitmapBytes;
};

#endif