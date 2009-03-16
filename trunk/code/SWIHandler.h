#ifndef _INC_SWIHANDLER
#define _INC_SWIHANDLER

#include "ARMRegisters.h"
#include "DMAController.h"
#include "types.h"

class SWIHandler
{
public:
	SWIHandler();
	~SWIHandler();

	void handleSWI( uint32 swiNum, ARMRegisters* reg, DMAController* DMA );
};

#endif