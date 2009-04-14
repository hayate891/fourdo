#include "KernelFaker.hpp"
#include "BitMath.h"

#define FAKE_KERNEL_BASE   ( 0x00020230 ) // Memory location of fake kernel table.
#define KERNEL_OFFSET_MIN  (-0x0230 )	  // Minimum offset for an entry
#define KERNEL_OFFSET_MAX  (-0x0004 )     // Maximum offset for an entry
#define KERNEL_ENTRY_COUNT ( ( KERNEL_OFFSET_MAX - KERNEL_OFFSET_MIN) / 4 + 1 )

#define FAKE_HANDLER_BASE  ( 0x00022000 ) // Memory location for the start of the fake SWI handlers.
#define FAKE_HANDLER_SIZE  ( 2 )       // Size (in instructions) of each fake SWI handler.

#define SWI_INSTRUCTION_MASK ( 0xEF000000 )
#define RET_INSTRUCTION		 ( 0xE1A0F00E )

typedef struct
	{
	int		offset;
	int		swiNumber;
	} kernelTableEntry;

static kernelTableEntry kernelTable[] =
	{
		{ -0x78, SWI_NUM_IMAGE_ENTRY_POINT }, // Code that runs at image entry point
		
        { -0x3C, SWI_NUM_MEMCPY },
        { -0x38, SWI_NUM_MEMSET },
        { -0xB4, SWI_NUM_NEXTTAGARG },
        { -0xB0, SWI_NUM_FINDTAGARG },
        { -0xA4, SWI_NUM_CHECKIO },
        { -0x98, SWI_NUM_FINDIMAGE },
        { -0x64, SWI_NUM_FINDMH },
        { -0x60, SWI_NUM_TICKS2TIMEVAL },
        { -0x5C, SWI_NUM_GETSYSERR },
        { -0x58, SWI_NUM_VFPRINTF },
        { -0x4C, SWI_NUM_USEC2TICKS },
        { -0x48, SWI_NUM_FINSERT },
        { -0x40, SWI_NUM_GETPAGESIZE },
        
        { -0x1C, SWI_NUM_ALLOC_MEM },
        { -0x70, SWI_NUM_ALLOC_MEMLIST },
        { -0x68, SWI_NUM_ALLOC_MEMFROMMEMLIST },
        { -0x20, SWI_NUM_FREE_MEM },
        { -0x74, SWI_NUM_FREE_MEMLIST },
        { -0x6C, SWI_NUM_FREE_MEMFROMMEMLIST },
        { -0x2C, SWI_NUM_SCAVENGE_MEM },
        { -0x9C, SWI_NUM_GET_MEMALLOCALIGNMENT },
        { -0xA0, SWI_NUM_GET_MEMTRACKSIZE },
        { -0xA8, SWI_NUM_GET_ISMEMREADABLE },
        { -0xAC, SWI_NUM_GET_ISMEMWRITABLE },
        
        { -0x30, SWI_NUM_LOOKUPITEM },
        { -0x34, SWI_NUM_LOCATEITEM },
        { -0x44, SWI_NUM_CHECKITEM },
        { -0x80, SWI_NUM_ITEMOPENED },
        
        { -0x24, SWI_NUM_INITLIST },
        { -0x08, SWI_NUM_ADDHEAD },
        { -0x10, SWI_NUM_ADDTAIL },
        { -0x54, SWI_NUM_INSERTHEAD },
        { -0x14, SWI_NUM_INSERTTAIL },
        { -0x04, SWI_NUM_REMHEAD },
        { -0x0C, SWI_NUM_REMTAIL },
        { -0x18, SWI_NUM_REMOVENODE },
        { -0x50, SWI_NUM_SETNODEPRI },
        { -0x28, SWI_NUM_FINDNAMEDNODE },
    };

#define cnt_of_array(n) (sizeof(n)/sizeof(n[0])) 

//////////////////////////////////////////////////////
// Class implementation

KernelFaker::KernelFaker()
{
	// Nothing to do
}

KernelFaker::~KernelFaker()
{
	// Nothing to do
}

uint32 KernelFaker::getFakeKernelBase()
{
    return FAKE_KERNEL_BASE;
}

void KernelFaker::init( DMA* aDMA )
{
    mDMA = aDMA;
    
    this->InitializeFakeKernel();
}

void KernelFaker::InitializeFakeKernel()
{
	uint32* tablePtr;
	uint32* tablePtrBase;
	uint32* handlerPtr;
	uint32* handlerPtrBase;
	
	tablePtrBase   = (uint32*)mDMA->getPtr( FAKE_KERNEL_BASE );
	handlerPtrBase = (uint32*)mDMA->getPtr( FAKE_HANDLER_BASE );
	
	///////////////////////////////////////////
	// First, initialize all table entries to contain pointers to 
	// functions that have a single SWI in them.
	tablePtr = tablePtrBase + (KERNEL_OFFSET_MIN / 4);
	handlerPtr = handlerPtrBase;
	for( int functionNum = 0; functionNum < KERNEL_ENTRY_COUNT; functionNum++ )
	{
		*tablePtr = FAKE_HANDLER_BASE + (functionNum * (FAKE_HANDLER_SIZE*4));
		
		*(handlerPtr)   = SWI_INSTRUCTION_MASK | (SWI_NUM_UNKNOWN_KERNEL_ENTRY+functionNum);
		*(handlerPtr+1) = RET_INSTRUCTION;
		
		// Move to next function handler address.
		tablePtr++;
		handlerPtr += FAKE_HANDLER_SIZE;
	}
	
	///////////////////////////////////////////
	// Now give special SWI handlers to those defined
	for( int entryNum = 0; entryNum < cnt_of_array(kernelTable); entryNum++ )
	{
		kernelTableEntry entry = kernelTable[entryNum];
		int	functionNum = (entry.offset-KERNEL_OFFSET_MIN) / 4;
	 	
		// Find function pointer.
		handlerPtr = handlerPtrBase + (functionNum * FAKE_HANDLER_SIZE);
		
		// Overwrite with the given SWI number.
		*(handlerPtr) = SWI_INSTRUCTION_MASK | entry.swiNumber;
	}
}
