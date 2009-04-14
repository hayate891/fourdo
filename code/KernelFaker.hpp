#ifndef _INC_KERNELFAKER
#define _INC_KERNELFAKER

#include "types.h"
#include "DMA.hpp"

typedef uint32 swiCode; enum {
	SWI_NUM_UNKNOWN_KERNEL_ENTRY  = 0x0080000,
	SWI_NUM_IMAGE_ENTRY_POINT     = 0x0081000,
    
    // Misc
	SWI_NUM_MEMCPY                = 0x0011000,
	SWI_NUM_MEMSET                = 0x0011010,
    SWI_NUM_NEXTTAGARG            = 0x0011500,
    SWI_NUM_FINDTAGARG            = 0x0011510,
    SWI_NUM_CHECKIO               = 0x0011520,
    SWI_NUM_FINDIMAGE             = 0x0011530,
    SWI_NUM_FINDMH                = 0x0011540,
    SWI_NUM_TICKS2TIMEVAL         = 0x0011550,
    SWI_NUM_GETSYSERR             = 0x0011560,
    SWI_NUM_VFPRINTF              = 0x0011570,
    SWI_NUM_USEC2TICKS            = 0x0011580,
    SWI_NUM_FINSERT               = 0x0011590,
    SWI_NUM_GETPAGESIZE           = 0x0011595,

	// Mem-related
	SWI_NUM_ALLOC_MEM             = 0x0011100,
	SWI_NUM_ALLOC_MEMLIST         = 0x0011110,
	SWI_NUM_ALLOC_MEMFROMMEMLIST  = 0x0011120,
	SWI_NUM_FREE_MEM              = 0x0011200,
	SWI_NUM_FREE_MEMLIST          = 0x0011210,
	SWI_NUM_FREE_MEMFROMMEMLIST   = 0x0011220,
	SWI_NUM_SCAVENGE_MEM          = 0x0011500,
	SWI_NUM_GET_MEMALLOCALIGNMENT = 0x0011510,
	SWI_NUM_GET_MEMTRACKSIZE      = 0x0011520,
	SWI_NUM_GET_ISMEMREADABLE     = 0x0011530,
	SWI_NUM_GET_ISMEMWRITABLE     = 0x0011540,
	
	// Items
	SWI_NUM_LOOKUPITEM            = 0x0012000,
	SWI_NUM_LOCATEITEM            = 0x0012010,
	SWI_NUM_CHECKITEM             = 0x0012020,
	SWI_NUM_ITEMOPENED            = 0x0012030,

    // Lists?
    SWI_NUM_INITLIST              = 0x0013000,
    SWI_NUM_ADDHEAD               = 0x0013100,
    SWI_NUM_ADDTAIL               = 0x0013110,
    SWI_NUM_INSERTHEAD            = 0x0013200,
    SWI_NUM_INSERTTAIL            = 0x0013210,
    SWI_NUM_REMHEAD               = 0x0013300,
    SWI_NUM_REMTAIL               = 0x0013310,
    SWI_NUM_REMOVENODE            = 0x0013320,
    SWI_NUM_SETNODEPRI            = 0x0013500,
    SWI_NUM_FINDNAMEDNODE         = 0x0013510,
};

class KernelFaker
{
public:
	KernelFaker ();
	~KernelFaker();

	void init( DMA* aDMA );
	
	static uint32 getFakeKernelBase();

protected:
    void InitializeFakeKernel();
    
    DMA* mDMA;
};

#endif //_INC_KERNELFAKER
