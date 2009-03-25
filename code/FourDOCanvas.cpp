#include "FourDOCanvas.hpp"

FourDOCanvas::FourDOCanvas
		(
		wxWindow *parent, 
		wxWindowID id,
        int* attribList,
        const wxPoint& pos,
        const wxSize& size, 
        long style,
        const wxString& name,
        const wxPalette& palette
		)
	: wxGLCanvas( parent, id, attribList, pos, size, style, name, palette )
{
}