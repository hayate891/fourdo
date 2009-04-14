#include <string>
#include <vector>
#include <wx/string.h>

#include "BitMath.h"
#include "KernelFaker.hpp"
#include "SWIHandler.h"

#include "SWI_ARM.hpp"
#include "SWI_Meta.hpp"
#include "SWI_Kernel.hpp"

#pragma warning (disable : 4100) // Disable damned "unused param" warnings.

namespace fourdo { namespace swi
{
	// 
	// namespace scope initialization
	// 

	std::map<uint32, swiHandler> initSwiMap()
	{
		std::map<uint32, swiHandler> map;

		map.insert(std::pair<uint32, swiHandler>(0x00010, &fourdo::swi::SWI_ARM_GetMemoryBaseAddress));
		map.insert(std::pair<uint32, swiHandler>(0x00011, &fourdo::swi::SWI_ARM_HaltExecution));
		
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_IMAGE_ENTRY_POINT, &fourdo::swi::SWI_META_ImageEntryPoint));
		
		map.insert(std::pair<uint32, swiHandler>(0x1000e, &fourdo::swi::SWI_KRN_kprintf));
		
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_ALLOC_MEM,             &fourdo::swi::SWI_KRN_AllocMem));
		map.insert(std::pair<uint32, swiHandler>(0x1000d,                       &fourdo::swi::SWI_KRN_AllocMemBlocks));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_ALLOC_MEMFROMMEMLIST,  &fourdo::swi::SWI_KRN_AllocMemFromMemLists));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_ALLOC_MEMLIST,         &fourdo::swi::SWI_KRN_AllocMemList));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_FREE_MEM,              &fourdo::swi::SWI_KRN_FreeMem));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_FREE_MEMLIST,          &fourdo::swi::SWI_KRN_FreeMemList));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_FREE_MEMFROMMEMLIST,   &fourdo::swi::SWI_KRN_FreeMemToMemList));
        map.insert(std::pair<uint32, swiHandler>(0x10021,                       &fourdo::swi::SWI_KRN_ScavengeMem));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_SCAVENGE_MEM,          &fourdo::swi::SWI_KRN_ScavengeMem));
		map.insert(std::pair<uint32, swiHandler>(0x10014,                       &fourdo::swi::SWI_KRN_ControlMem));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_GET_MEMALLOCALIGNMENT, &fourdo::swi::SWI_KRN_GetMemAllocAlignment));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_GET_MEMTRACKSIZE,      &fourdo::swi::SWI_KRN_GetMemTrackSize));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_GETPAGESIZE,           &fourdo::swi::SWI_KRN_GetPageSize));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_GET_ISMEMREADABLE,     &fourdo::swi::SWI_KRN_IsMemReadable));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_GET_ISMEMWRITABLE,     &fourdo::swi::SWI_KRN_IsMemWritable));

		return map;
	}

	std::map<uint32, swiHandler> swiToHandlerMap = initSwiMap();
	
	///////////////////////////////////////////////////////////////
	// ARM Interrupts
	// 
	void SWI_ARM_GetMemoryBaseAddress(ARMRegisters* registers, DMA* dma)
	{
		//registers->USER[0] = ARM_GetMemoryBaseAddress();
	}
	
	void SWI_ARM_HaltExecution(ARMRegisters* registers, DMA* dma )
	{
		//ARM_HaltExecution();
	}

	///////////////////////////////////////////////////////////////
	// Meta Interrupts
	//
	void SWI_META_ImageEntryPoint(ARMRegisters* registers, DMA* dma )
	{
		//META_ImageEntryPoint();
	}
	
	///////////////////////////////////////////////////////////////
	// Kernel Interrupts
	//
	
	// kprintf
	void SWI_KRN_kprintf(ARMRegisters *registers, DMA *dma)
	{
		//std::string format(reinterpret_cast<const char *>(dma->getPtr(registers->USER[0])));
		std::string format;
        
        format = ReadSwappedString((char*)(dma->getPtr(registers->USER[0])));
        
		// 
		// parse the format string to determine how many more arguments 
		// we need to get from registers and the stack
		// 
		size_t substitution = format.find('%');
		std::vector<void *> argv;

		while (substitution != std::string::npos)
		{
			switch (format.at(substitution + 1))
			{
				case 's':
					if (argv.size() >= 3)
						argv.push_back(
							static_cast<void *>(
								dma->getPtr(registers->USER[13] + ((argv.size() - 3) * 4))
							)
						);
					else
						argv.push_back(
							static_cast<void *>(
								dma->getPtr(registers->USER[argv.size() + 1])
							)
						);
					break;
				case '%':
					break;
				default:
					if (argv.size() >= 3)
					{
						argv.push_back(
							static_cast<void *>(
								dma->getPtr(registers->USER[13] + ((argv.size() - 3) * 4))
							)
						);
					}
					else
						argv.push_back(static_cast<void *>(&registers->USER[argv.size() + 1]));

					break;
			}

			substitution = format.find('%', substitution + 1);
		}

		// 
		// execute our swi
		// 
		KRN_kprintf(
			format.c_str(), 
			argv.size(), 
			(argv.size() ? &argv.front() : NULL));
	}
	
	/////////////////////////////////////
	// Memory
	
	// Mem allocation
	
	void SWI_KRN_malloc              (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_malloc( registers->USER[1] );
	}
	
	void SWI_KRN_AllocMem            (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_AllocMem( registers->USER[1], registers->USER[2] );
	}

	void SWI_KRN_AllocMemBlocks      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_AllocMemBlocks( registers->USER[1], registers->USER[2] );
	}

	void SWI_KRN_AllocMemFromMemList (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_AllocMemFromMemList( registers->USER[1], registers->USER[2], registers->USER[3] );
	}

	void SWI_KRN_AllocMemFromMemLists(ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_AllocMemFromMemLists( registers->USER[1], registers->USER[2], registers->USER[3] );
	}

	void SWI_KRN_AllocMemList        (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_AllocMemList( registers->USER[1], registers->USER[2] );
	}


	// Deletion
	void SWI_KRN_free             (ARMRegisters *registers, DMA *dma)
	{
		KRN_free( registers->USER[0] );
	}

	void SWI_KRN_FreeMem          (ARMRegisters *registers, DMA *dma)
	{
		KRN_FreeMem( registers->USER[0], registers->USER[1] );
	}

	void SWI_KRN_FreeMemList      (ARMRegisters *registers, DMA *dma)
	{
		KRN_FreeMemList( registers->USER[0] );
	}

	void SWI_KRN_FreeMemToMemList (ARMRegisters *registers, DMA *dma)
	{
		KRN_FreeMemToMemList( registers->USER[0], registers->USER[1], registers->USER[2] );
	}

	void SWI_KRN_FreeMemToMemLists(ARMRegisters *registers, DMA *dma)
	{
		KRN_FreeMemToMemLists( registers->USER[0], registers->USER[1], registers->USER[2] );
	}

	void SWI_KRN_ScavengeMem      (ARMRegisters *registers, DMA *dma)
	{
		 registers->USER[0] = KRN_ScavengeMem();
	}


	// Memory info
	void SWI_KRN_AvailMem            (ARMRegisters *registers, DMA *dma)
	{
		KRN_AvailMem( registers->USER[0], registers->USER[1] );
	}

	void SWI_KRN_ControlMem          (ARMRegisters *registers, DMA *dma)
	{
		// DANGER! This is probably supposed to read the last param from the stack!
		registers->USER[0] = KRN_ControlMem( registers->USER[1], registers->USER[2], registers->USER[3], registers->USER[4] );
	}

	void SWI_KRN_GetMemAllocAlignment(ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_GetMemAllocAlignment( registers->USER[1] );
	}

	void SWI_KRN_GetMemTrackSize     (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_GetMemTrackSize( registers->USER[1] );
	}

	void SWI_KRN_GetMemType          (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_GetMemType( registers->USER[1] );
	}

	void SWI_KRN_GetPageSize         (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_GetPageSize( registers->USER[1] );
	}

	void SWI_KRN_IsMemReadable       (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_IsMemReadable( registers->USER[1], registers->USER[2] );
	}

	void SWI_KRN_IsMemWritable       (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_IsMemWritable( registers->USER[1], registers->USER[2] );
	}


	// Memory debugging
	void SWI_KRN_DumpMemDebug        (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_DumpMemDebug( registers->USER[1] );
	}

	void SWI_KRN_CreateMemDebug      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_CreateMemDebug( registers->USER[1], registers->USER[2] );
	}

	void SWI_KRN_DeleteMemDebug      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_DeleteMemDebug();
	}

	void SWI_KRN_SanityCheckMemDebug (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_SanityCheckMemDebug( registers->USER[1] );
	}
	
	/////////////////////////////////////
	// Items
	
	// Creating items
	
    void SWI_KRN_CreateBufferedMsg     (ARMRegisters *registers, DMA *dma)
	{
	}

    void SWI_KRN_CreateIOReq           (ARMRegisters *registers, DMA *dma)
	{
	}

    void SWI_KRN_CreateItem            (ARMRegisters *registers, DMA *dma)
	{
	}

    void SWI_KRN_CreateItemVA          (ARMRegisters *registers, DMA *dma)
	{
	}

    void SWI_KRN_CreateMsg             (ARMRegisters *registers, DMA *dma)
	{
	}

    void SWI_KRN_CreateMsgPort         (ARMRegisters *registers, DMA *dma)
	{
	}

    void SWI_KRN_CreateSemaphore       (ARMRegisters *registers, DMA *dma)
	{
	}

    void SWI_KRN_CreateSmallMsg        (ARMRegisters *registers, DMA *dma)
	{
	}

    void SWI_KRN_CreateThread          (ARMRegisters *registers, DMA *dma)
	{
	}

    void SWI_KRN_CreateUniqueMsgPort   (ARMRegisters *registers, DMA *dma)
	{
	}

    void SWI_KRN_CreateUniqueSemaphore (ARMRegisters *registers, DMA *dma)
	{
	}


    // Opening Items
    
    void SWI_KRN_FindAndOpenDevice     (ARMRegisters *registers, DMA *dma)
	{
	}

    void SWI_KRN_FindAndOpenFolio      (ARMRegisters *registers, DMA *dma)
	{
	}

    void SWI_KRN_FindAndOpenItem       (ARMRegisters *registers, DMA *dma)
	{
	}

    void SWI_KRN_FindAndOpenItemVA     (ARMRegisters *registers, DMA *dma)
	{
	}

    void SWI_KRN_FindAndOpenNamedItem  (ARMRegisters *registers, DMA *dma)
	{
	}

    void SWI_KRN_OpenItem              (ARMRegisters *registers, DMA *dma)
	{
	}

    void SWI_KRN_OpenNamedDevice       (ARMRegisters *registers, DMA *dma)
	{
	}


    // Managing Items
    
    void SWI_KRN_CheckItem             (ARMRegisters *registers, DMA *dma)
	{
	}

    void SWI_KRN_FindDevice            (ARMRegisters *registers, DMA *dma)
	{
	}

    void SWI_KRN_FindFolio             (ARMRegisters *registers, DMA *dma)
	{
	}

    void SWI_KRN_FindItem              (ARMRegisters *registers, DMA *dma)
	{
	}

    void SWI_KRN_FindItemVA            (ARMRegisters *registers, DMA *dma)
	{
	}

    void SWI_KRN_FindMsgPort           (ARMRegisters *registers, DMA *dma)
	{
	}

    void SWI_KRN_FindNamedItem         (ARMRegisters *registers, DMA *dma)
	{
	}

    void SWI_KRN_FindSemaphore         (ARMRegisters *registers, DMA *dma)
	{
	}

    void SWI_KRN_FindTask              (ARMRegisters *registers, DMA *dma)
	{
	}

    void SWI_KRN_FindVersionedItem     (ARMRegisters *registers, DMA *dma)
	{
	}

    void SWI_KRN_LookupItem            (ARMRegisters *registers, DMA *dma)
	{
	}

    void SWI_KRN_SetItemOwner          (ARMRegisters *registers, DMA *dma)
	{
	}

    void SWI_KRN_SetItemPri            (ARMRegisters *registers, DMA *dma)
	{
	}


    // Closing and Deleting Items
    
    void SWI_KRN_CloseItem             (ARMRegisters *registers, DMA *dma)
	{
	}

    void SWI_KRN_CloseNamedDevice      (ARMRegisters *registers, DMA *dma)
	{
	}

    void SWI_KRN_DeleteIOReq           (ARMRegisters *registers, DMA *dma)
	{
	}

    void SWI_KRN_DeleteItem            (ARMRegisters *registers, DMA *dma)
	{
	}

    void SWI_KRN_DeleteMsg             (ARMRegisters *registers, DMA *dma)
	{
	}

    void SWI_KRN_DeleteMsgPort         (ARMRegisters *registers, DMA *dma)
	{
	}

    void SWI_KRN_DeleteSemaphore       (ARMRegisters *registers, DMA *dma)
	{
	}

    void SWI_KRN_DeleteThread          (ARMRegisters *registers, DMA *dma)
	{
	}


}
}

#pragma warning (default : 4100)