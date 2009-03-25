#ifndef _INC_FOURDOCANVAS
#define _INC_FOURDOCANVAS

#include "wx/wx.h"
#include "wx/glcanvas.h"

class FourDOCanvas : public wxGLCanvas
{
public:
    FourDOCanvas
		(
		wxWindow *parent, 
		wxWindowID id = wxID_ANY,
        int* attribList = 0,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize, 
        long style = 0,
        const wxString& name = wxGLCanvasName,
        const wxPalette& palette = wxNullPalette
		);
};

#endif