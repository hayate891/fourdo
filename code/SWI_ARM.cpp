#include "SWI_ARM.hpp"

// SWI 0x10
uint32 ARM_GetMemoryBaseAddress()
{
	//////////////////////////////////////////////////////////////////////////
	// The legendary tale of FourDO's first fully 100% implemented SWI
	//////////////////////////////////////////////////////////////////////////
	// This is called by all of 3DO's ARM images! I ignored it and only
	// discovered it's true purpose after several months.
	//
	// This is a SWI used by code that is added by the ARM linker when code
	// uses the self-move feature of the self-relocation logic. I've been
	// told that all 3DO code is self-relocating. Apparently no code is
	// self-moving, however.
	//
	// The 3DO code avoids the self-move logic by setting the images'
	// reported size to an impossibly large value to make the self-move
	// logic break out early and abort the move.
	//
	// The 3DO always just returns zero for this function. Ultimately, any 
	// other value will do since the self-move logic exits anyway.
	//////////////////////////////////////////////////////////////////////////
	return 0;
}

// SWI 0x11
void ARM_HaltExecution()
{
	// This SWI is supposed to indicate that the running program has requested
	// to halt execution. I don't imagine I'll ever see it come up.
	
	// TODO: Reset console.
}