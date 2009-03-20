#ifndef _INC_KERNELFAKER
#define _INC_KERNELFAKER

#include "types.h"
#include "DMAController.h"

typedef uint32 swiCode; enum {
	SWI_NUM_UNKNOWN_KERNEL_ENTRY = 0x0080000,
	SWI_NUM_IMAGE_ENTRY_POINT    = 0x0080100,
};

class KernelFaker
{
public:
	KernelFaker ();
	~KernelFaker();

	uint32 getFakeKernelBase();
	void InitializeFakeKernel( DMAController* DMA );
};

#endif //_INC_KERNELFAKER
