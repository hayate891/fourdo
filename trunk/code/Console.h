#ifndef _INC_CONSOLE
#define _INC_CONSOLE

#include "wx/wx.h"

#include "types.h"
#include "ARMCPU.h"
#include "DMAController.h"
#include "KernelFaker.hpp"

//////////////////////////////////////////////////

class Console
{
public:
   Console  ();
   ~Console ();
   
   ARMCPU*         CPU ();
   DMAController*  DMA ();
   
   bool loadIso( wxString fileName );
   bool loadBinary( wxString fileName );
   
   void Reset();

protected:
   ARMCPU*         mCPU;
   DMAController*  mDMA;
   KernelFaker*    mKernelFaker;
};

#endif //_INC_CONSOLE