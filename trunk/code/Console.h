#ifndef _INC_CONSOLE
#define _INC_CONSOLE

#include "wx/wx.h"

#include "types.h"
#include "ARMCPU.h"
#include "DMA.hpp"
#include "KernelFaker.hpp"

//////////////////////////////////////////////////

class Console
{
public:
   Console  ();
   ~Console ();
   
   ARMCPU*         CPU ();
   
   bool loadIso( wxString fileName );
   bool loadBinary( wxString fileName );
   
   void Reset();

protected:
   ARMCPU*         mCPU;
   KernelFaker*    mKernelFaker;
};

#endif //_INC_CONSOLE