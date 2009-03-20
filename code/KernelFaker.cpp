#include "KernelFaker.h"

#define FAKE_KERNEL_BASE   ( 0x00021230 ) // Memory location of fake kernel table.
#define KERNEL_OFFSET_MIN  (-0x0230 )	  // Minimum offset for an entry
#define KERNEL_OFFSET_MAX  ( 0x01FC )     // Maximum offset for an entry
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
	return (uint32)FAKE_KERNEL_BASE;
}

void KernelFaker::InitializeFakeKernel( DMAController* DMA )
{
	uint32* tablePtr;
	uint32* tablePtrBase;
	uint32* handlerPtr;
	uint32* handlerPtrBase;
	
	tablePtrBase   = (uint32*)DMA->GetRAMPointer( FAKE_KERNEL_BASE + KERNEL_OFFSET_MIN );
	handlerPtrBase = (uint32*)DMA->GetRAMPointer( FAKE_HANDLER_BASE );
	
	///////////////////////////////////////////
	// First, initialize all table entries to contain pointers to 
	// functions that have a single SWI in them.
	tablePtr = tablePtrBase + (KERNEL_OFFSET_MIN / 4);
	handlerPtr = handlerPtrBase;
	for( int functionNum = 0; functionNum < KERNEL_ENTRY_COUNT; functionNum++ )
	{
		*tablePtr = FAKE_HANDLER_BASE + (functionNum * (FAKE_HANDLER_SIZE*4));
		
		*(handlerPtr)   = SWI_INSTRUCTION_MASK | SWI_NUM_UNKNOWN_KERNEL_ENTRY;
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
		
		// Find function pointer.
		handlerPtr = handlerPtrBase + ((entry.offset-KERNEL_OFFSET_MIN) / 4);
		
		// Overwrite with the given SWI number.
		*(handlerPtr) = SWI_INSTRUCTION_MASK | entry.swiNumber;
	}
}