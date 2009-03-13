#ifndef _INC_CONSOLE
#define _INC_CONSOLE

#include "types.h"
#include "ARMCPU.h"
#include "DMAController.h"

//////////////////////////////////////////////////

class Console
{
public:
   Console  ();
   ~Console ();
   
   ARMCPU*         CPU ();
   DMAController*  DMA ();

private:
   ARMCPU*         m_CPU;
   DMAController*  m_DMA;
   
};

#endif //_INC_CONSOLE