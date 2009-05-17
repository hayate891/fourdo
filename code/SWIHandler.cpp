#include <string>
#include <vector>
#include <wx/string.h>

#include "BitMath.h"
#include "KernelFaker.hpp"
#include "SWIHandler.h"

#include "SWI_ARM.hpp"
#include "SWI_Meta.hpp"
#include "SWI_Kernel.hpp"

#include "SWI_NUM_Stubs.hpp"

#pragma warning (disable : 4100) // Disable damned "unused param" warnings.

namespace fourdo { namespace swi
{
	// 
	// namespace scope initialization
	// 

	std::map<uint32, swiHandler> initSwiMap()
	{
		std::map<uint32, swiHandler> map;

	// ARM

		map.insert(std::pair<uint32, swiHandler>(0x00010,						&fourdo::swi::SWI_ARM_GetMemoryBaseAddress));
		map.insert(std::pair<uint32, swiHandler>(0x00011,						&fourdo::swi::SWI_ARM_HaltExecution));

	// META

		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_IMAGE_ENTRY_POINT,		&fourdo::swi::SWI_META_ImageEntryPoint));

	// KRN

		map.insert(std::pair<uint32, swiHandler>(0x10019,						&fourdo::swi::SWI_KRN_AbortIO));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_AddHead,				&fourdo::swi::SWI_KRN_AddHead));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_AddTail,				&fourdo::swi::SWI_KRN_AddTail));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_AllocMem,				&fourdo::swi::SWI_KRN_AllocMem));
		map.insert(std::pair<uint32, swiHandler>(0x1000d,						&fourdo::swi::SWI_KRN_AllocMemBlocks));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_AllocMemFromMemList,   &fourdo::swi::SWI_KRN_AllocMemFromMemList));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_AllocMemFromMemLists,  &fourdo::swi::SWI_KRN_AllocMemFromMemLists));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_AllocMemList,          &fourdo::swi::SWI_KRN_AllocMemList));
		map.insert(std::pair<uint32, swiHandler>(0x10015,						&fourdo::swi::SWI_KRN_AllocSignal));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_AvailMem,				&fourdo::swi::SWI_KRN_AvailMem));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_CallFolio,				&fourdo::swi::SWI_KRN_CallFolio));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_CallFolioRet,			&fourdo::swi::SWI_KRN_CallFolioRet));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_CheckIO,				&fourdo::swi::SWI_KRN_CheckIO));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_CheckItem,				&fourdo::swi::SWI_KRN_CheckItem));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_ClearCurrentSignals,   &fourdo::swi::SWI_KRN_ClearCurrentSignals));
		map.insert(std::pair<uint32, swiHandler>(0x10008,						&fourdo::swi::SWI_KRN_CloseItem));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_CloseNamedDevice,      &fourdo::swi::SWI_KRN_CloseNamedDevice));
		map.insert(std::pair<uint32, swiHandler>(0x10022,						&fourdo::swi::SWI_KRN_CompleteIO));
		map.insert(std::pair<uint32, swiHandler>(0x10014,						&fourdo::swi::SWI_KRN_ControlMem));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_CountBits,				&fourdo::swi::SWI_KRN_CountBits));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_CreateBufferedMsg,     &fourdo::swi::SWI_KRN_CreateBufferedMsg));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_CreateIOReq,			&fourdo::swi::SWI_KRN_CreateIOReq));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_CreateItem,			&fourdo::swi::SWI_KRN_CreateItem));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_CreateItemVA,			&fourdo::swi::SWI_KRN_CreateItemVA));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_CreateMemDebug,        &fourdo::swi::SWI_KRN_CreateMemDebug));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_CreateMsg,				&fourdo::swi::SWI_KRN_CreateMsg));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_CreateMsgPort,         &fourdo::swi::SWI_KRN_CreateMsgPort));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_CreateSemaphore,       &fourdo::swi::SWI_KRN_CreateSemaphore));
		map.insert(std::pair<uint32, swiHandler>(0x10000,						&fourdo::swi::SWI_KRN_CreateSizedItem));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_CreateSmallMsg,        &fourdo::swi::SWI_KRN_CreateSmallMsg));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_CreateThread,			&fourdo::swi::SWI_KRN_CreateThread));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_CreateUniqueMsgPort,   &fourdo::swi::SWI_KRN_CreateUniqueMsgPort));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_CreateUniqueSemaphore, &fourdo::swi::SWI_KRN_CreateUniqueSemaphore));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_DeleteIOReq,			&fourdo::swi::SWI_KRN_DeleteIOReq));
		map.insert(std::pair<uint32, swiHandler>(0x10003,						&fourdo::swi::SWI_KRN_DeleteItem));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_DeleteMemDebug,        &fourdo::swi::SWI_KRN_DeleteMemDebug));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_DeleteMsg,				&fourdo::swi::SWI_KRN_DeleteMsg));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_DeleteMsgPort,         &fourdo::swi::SWI_KRN_DeleteMsgPort));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_DeleteSemaphore,       &fourdo::swi::SWI_KRN_DeleteSemaphore));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_DeleteThread,			&fourdo::swi::SWI_KRN_DeleteThread));
		map.insert(std::pair<uint32, swiHandler>(0x10025,						&fourdo::swi::SWI_KRN_DoIO));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_DumpMemDebug,			&fourdo::swi::SWI_KRN_DumpMemDebug));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_DumpNode,				&fourdo::swi::SWI_KRN_DumpNode));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_DumpTagList,			&fourdo::swi::SWI_KRN_DumpTagList));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_ffs,					&fourdo::swi::SWI_KRN_ffs));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_FindAndOpenDevice,     &fourdo::swi::SWI_KRN_FindAndOpenDevice));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_FindAndOpenFolio,      &fourdo::swi::SWI_KRN_FindAndOpenFolio));
		map.insert(std::pair<uint32, swiHandler>(0x10024,						&fourdo::swi::SWI_KRN_FindAndOpenItem));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_FindAndOpenNamedItem,  &fourdo::swi::SWI_KRN_FindAndOpenNamedItem));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_FindAndOpenItemVA,		&fourdo::swi::SWI_KRN_FindAndOpenItemVA));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_FindDevice,			&fourdo::swi::SWI_KRN_FindDevice));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_FindFolio,				&fourdo::swi::SWI_KRN_FindFolio));
		map.insert(std::pair<uint32, swiHandler>(0x10004,						&fourdo::swi::SWI_KRN_FindItem));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_FindItemVA,			&fourdo::swi::SWI_KRN_FindItemVA));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_FindLSB,				&fourdo::swi::SWI_KRN_FindLSB));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_FindMSB,				&fourdo::swi::SWI_KRN_FindMSB));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_FindMsgPort,			&fourdo::swi::SWI_KRN_FindMsgPort));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_FindNamedItem,         &fourdo::swi::SWI_KRN_FindNamedItem));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_FindNamedNode,         &fourdo::swi::SWI_KRN_FindNamedNode));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_FindNodeFromHead,      &fourdo::swi::SWI_KRN_FindNodeFromHead));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_FindNodeFromTail,      &fourdo::swi::SWI_KRN_FindNodeFromTail));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_FindSemaphore,         &fourdo::swi::SWI_KRN_FindSemaphore));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_FindTagArg,			&fourdo::swi::SWI_KRN_FindTagArg));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_FindTask,				&fourdo::swi::SWI_KRN_FindTask));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_FindVersionedItem,     &fourdo::swi::SWI_KRN_FindVersionedItem));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_FirstNode,				&fourdo::swi::SWI_KRN_FirstNode));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_free,					&fourdo::swi::SWI_KRN_free));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_FreeMem,				&fourdo::swi::SWI_KRN_FreeMem));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_FreeMemList,			&fourdo::swi::SWI_KRN_FreeMemList));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_FreeMemToMemList,      &fourdo::swi::SWI_KRN_FreeMemToMemList));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_FreeMemToMemLists,     &fourdo::swi::SWI_KRN_FreeMemToMemLists));
		map.insert(std::pair<uint32, swiHandler>(0x10016,						&fourdo::swi::SWI_KRN_FreeSignal));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_GetBankBits,			&fourdo::swi::SWI_KRN_GetBankBits));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_GetCurrentSignals,     &fourdo::swi::SWI_KRN_GetCurrentSignals));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_GetFolioFunc,			&fourdo::swi::SWI_KRN_GetFolioFunc));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_GetMemAllocAlignment,  &fourdo::swi::SWI_KRN_GetMemAllocAlignment));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_GetMemTrackSize,       &fourdo::swi::SWI_KRN_GetMemTrackSize));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_GetMemType,			&fourdo::swi::SWI_KRN_GetMemType));
		map.insert(std::pair<uint32, swiHandler>(0x10013,						&fourdo::swi::SWI_KRN_GetMsg));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_GetNodeCount,			&fourdo::swi::SWI_KRN_GetNodeCount));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_GetNodePosFromHead,    &fourdo::swi::SWI_KRN_GetNodePosFromHead));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_GetNodePosFromTail,    &fourdo::swi::SWI_KRN_GetNodePosFromTail));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_GetPageSize,			&fourdo::swi::SWI_KRN_GetPageSize));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_GetSysErr,				&fourdo::swi::SWI_KRN_GetSysErr));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_GetTagArg,				&fourdo::swi::SWI_KRN_GetTagArg));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_GetTaskSignals,        &fourdo::swi::SWI_KRN_GetTaskSignals));
		map.insert(std::pair<uint32, swiHandler>(0x1000f,						&fourdo::swi::SWI_KRN_GetThisMsg));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_InitList,				&fourdo::swi::SWI_KRN_InitList));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_InsertNodeAfter,       &fourdo::swi::SWI_KRN_InsertNodeAfter));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_InsertNodeBefore,      &fourdo::swi::SWI_KRN_InsertNodeBefore));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_InsertNodeFromHead,    &fourdo::swi::SWI_KRN_InsertNodeFromHead));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_InsertNodeFromTail,    &fourdo::swi::SWI_KRN_InsertNodeFromTail));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_IsEmptyList,			&fourdo::swi::SWI_KRN_IsEmptyList));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_IsItemOpened,			&fourdo::swi::SWI_KRN_IsItemOpened));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_IsListEmpty,			&fourdo::swi::SWI_KRN_IsListEmpty));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_IsMemReadable,         &fourdo::swi::SWI_KRN_IsMemReadable));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_IsMemWritable,         &fourdo::swi::SWI_KRN_IsMemWritable));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_IsNode,				&fourdo::swi::SWI_KRN_IsNode));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_IsNodeB,				&fourdo::swi::SWI_KRN_IsNodeB));
		map.insert(std::pair<uint32, swiHandler>(0x1000e,						&fourdo::swi::SWI_KRN_kprintf));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_LastNode,				&fourdo::swi::SWI_KRN_LastNode));
		map.insert(std::pair<uint32, swiHandler>(0x10007,						&fourdo::swi::SWI_KRN_LockItem));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_LockSemaphore,         &fourdo::swi::SWI_KRN_LockSemaphore));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_LookupItem,			&fourdo::swi::SWI_KRN_LookupItem));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_malloc,				&fourdo::swi::SWI_KRN_malloc));
		map.insert(std::pair<uint32, swiHandler>(0x1001e,						&fourdo::swi::SWI_KRN_MayGetChar));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_MkNodeID,				&fourdo::swi::SWI_KRN_MkNodeID));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_NextNode,				&fourdo::swi::SWI_KRN_NextNode));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_NextTagArg,			&fourdo::swi::SWI_KRN_NextTagArg));
		map.insert(std::pair<uint32, swiHandler>(0x10005,						&fourdo::swi::SWI_KRN_OpenItem));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_OpenNamedDevice,       &fourdo::swi::SWI_KRN_OpenNamedDevice));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_PrevNode,				&fourdo::swi::SWI_KRN_PrevNode));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_PrintError,			&fourdo::swi::SWI_KRN_PrintError));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_PrintfSysErr,			&fourdo::swi::SWI_KRN_PrintfSysErr));
		map.insert(std::pair<uint32, swiHandler>(0x10011,						&fourdo::swi::SWI_KRN_ReadHardwareRandomNumber));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_RemHead,				&fourdo::swi::SWI_KRN_RemHead));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_RemNode,				&fourdo::swi::SWI_KRN_RemNode));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_RemTail,				&fourdo::swi::SWI_KRN_RemTail));
		map.insert(std::pair<uint32, swiHandler>(0x10012,						&fourdo::swi::SWI_KRN_ReplyMsg));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_ReplySmallMsg,         &fourdo::swi::SWI_KRN_ReplySmallMsg));
		map.insert(std::pair<uint32, swiHandler>(0x10026,						&fourdo::swi::SWI_KRN_SampleSystemTime));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_SampleSystemTimeTV,    &fourdo::swi::SWI_KRN_SampleSystemTimeTV));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_SanityCheckMemDebug,   &fourdo::swi::SWI_KRN_SanityCheckMemDebug));
		map.insert(std::pair<uint32, swiHandler>(0x10021,						&fourdo::swi::SWI_KRN_ScavengeMem));
		map.insert(std::pair<uint32, swiHandler>(0x10018,						&fourdo::swi::SWI_KRN_SendIO));
		map.insert(std::pair<uint32, swiHandler>(0x10010,						&fourdo::swi::SWI_KRN_SendMsg));
		map.insert(std::pair<uint32, swiHandler>(0x10002,						&fourdo::swi::SWI_KRN_SendSignal));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_SendSmallMsg,			&fourdo::swi::SWI_KRN_SendSmallMsg));
		map.insert(std::pair<uint32, swiHandler>(0x10027,						&fourdo::swi::SWI_KRN_SetExitStatus));
		map.insert(std::pair<uint32, swiHandler>(0x10017,						&fourdo::swi::SWI_KRN_SetFunction));
		map.insert(std::pair<uint32, swiHandler>(0x1001c,						&fourdo::swi::SWI_KRN_SetItemOwner));
		map.insert(std::pair<uint32, swiHandler>(0x1000a,						&fourdo::swi::SWI_KRN_SetItemPri));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_SetNodePri,			&fourdo::swi::SWI_KRN_SetNodePri));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_UniversalInsertNode,   &fourdo::swi::SWI_KRN_UniversalInsertNode));
		map.insert(std::pair<uint32, swiHandler>(0x10006,						&fourdo::swi::SWI_KRN_UnlockItem));
		map.insert(std::pair<uint32, swiHandler>(SWI_NUM_UnlockSemaphore,       &fourdo::swi::SWI_KRN_UnlockSemaphore));
		map.insert(std::pair<uint32, swiHandler>(0x10029,						&fourdo::swi::SWI_KRN_WaitIO));
		map.insert(std::pair<uint32, swiHandler>(0x10028,						&fourdo::swi::SWI_KRN_WaitPort));
		map.insert(std::pair<uint32, swiHandler>(0x10001,						&fourdo::swi::SWI_KRN_WaitSignal));
		map.insert(std::pair<uint32, swiHandler>(0x10009,						&fourdo::swi::SWI_KRN_Yield));

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
	
 	void SWI_KRN_AddHead      (ARMRegisters *registers, DMA *dma)
	{
		KRN_AddHead( registers->USER[0], registers->USER[1] );
	}

 	void SWI_KRN_AddTail      (ARMRegisters *registers, DMA *dma)
	{
		KRN_AddTail( registers->USER[0], registers->USER[1] );
	}

 	void SWI_KRN_CreateBufferedMsg      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_CreateBufferedMsg( registers->USER[1], registers->USER[2], registers->USER[3], registers->USER[4] );
	}

 	void SWI_KRN_CreateIOReq      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_CreateIOReq( registers->USER[1], registers->USER[2], registers->USER[3], registers->USER[4] );
	}

 	void SWI_KRN_CreateItem      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_CreateItem( registers->USER[1], registers->USER[2] );
	}

    void SWI_KRN_CreateItemVA          (ARMRegisters *registers, DMA *dma)
	{
	}

 	void SWI_KRN_CreateMsg      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_CreateMsg( registers->USER[1], registers->USER[2], registers->USER[3] );
	}

 	void SWI_KRN_CreateMsgPort      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_CreateMsgPort( registers->USER[1], registers->USER[2], registers->USER[3] );
	}

 	void SWI_KRN_CreateSemaphore      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_CreateSemaphore( registers->USER[1], registers->USER[2] );
	}

 	void SWI_KRN_CreateSizedItem      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_CreateSizedItem( registers->USER[1], registers->USER[2], registers->USER[3] );
	}

 	void SWI_KRN_CreateSmallMsg      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_CreateSmallMsg( registers->USER[1], registers->USER[2], registers->USER[3] );
	}

 	void SWI_KRN_CreateThread      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_CreateThread( registers->USER[1], registers->USER[2], registers->USER[3], registers->USER[4] );
	}

 	void SWI_KRN_CreateUniqueMsgPort      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_CreateUniqueMsgPort( registers->USER[1], registers->USER[2], registers->USER[3] );
	}

 	void SWI_KRN_CreateUniqueSemaphore      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_CreateUniqueSemaphore( registers->USER[1], registers->USER[2] );
	}

 	void SWI_KRN_InsertNodeAfter      (ARMRegisters *registers, DMA *dma)
	{
		KRN_InsertNodeAfter( registers->USER[0], registers->USER[1] );
	}

 	void SWI_KRN_InsertNodeBefore      (ARMRegisters *registers, DMA *dma)
	{
		KRN_InsertNodeBefore( registers->USER[0], registers->USER[1] );
	}

 	void SWI_KRN_InsertNodeFromHead      (ARMRegisters *registers, DMA *dma)
	{
		KRN_InsertNodeFromHead( registers->USER[0], registers->USER[1] );
	}

 	void SWI_KRN_InsertNodeFromTail      (ARMRegisters *registers, DMA *dma)
	{
		KRN_InsertNodeFromTail( registers->USER[0], registers->USER[1] );
	}

 	void SWI_KRN_MkNodeID      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_MkNodeID( registers->USER[1], registers->USER[2] );
	}

	void SWI_KRN_UniversalInsertNode      (ARMRegisters *registers, DMA *dma)
	{
		KRN_UniversalInsertNode( registers->USER[0], registers->USER[1], registers->USER[2] );
	}

    // Opening Items
    
 	void SWI_KRN_FindAndOpenDevice      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_FindAndOpenDevice( registers->USER[1] );
	}

 	void SWI_KRN_FindAndOpenFolio      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_FindAndOpenFolio( registers->USER[1] );
	}

 	void SWI_KRN_FindAndOpenItem      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_FindAndOpenItem( registers->USER[1], registers->USER[2] );
	}

    void SWI_KRN_FindAndOpenItemVA     (ARMRegisters *registers, DMA *dma)
	{
	}

 	void SWI_KRN_FindAndOpenNamedItem      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_FindAndOpenNamedItem( registers->USER[1], registers->USER[2] );
	}

 	void SWI_KRN_OpenItem      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_OpenItem( registers->USER[1], registers->USER[2] );
	}

 	void SWI_KRN_OpenNamedDevice      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_OpenNamedDevice( registers->USER[1], registers->USER[2] );
	}


    // Managing Items
    
    void SWI_KRN_AllocSignal      (ARMRegisters *registers, DMA *dma)
    {
		registers->USER[0] = KRN_AllocSignal( registers->USER[1] );
	}

 	void SWI_KRN_CallFolio      (ARMRegisters *registers, DMA *dma)
	{
		//registers->USER[0] = KRN_CallFolio( registers->USER[1], registers->USER[2], registers->USER[3] );
	}

 	void SWI_KRN_CallFolioRet      (ARMRegisters *registers, DMA *dma)
	{
		//registers->USER[0] = KRN_CallFolioRet( registers->USER[1], registers->USER[2], registers->USER[3], registers->USER[4], registers->[5] );
	}
    
     	void SWI_KRN_CheckIO      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_CheckIO( registers->USER[1] );
	}
    
 	void SWI_KRN_CheckItem      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_CheckItem( registers->USER[1], registers->USER[2], registers->USER[3] );
	}

 	void SWI_KRN_CompleteIO      (ARMRegisters *registers, DMA *dma)
	{
		KRN_CompleteIO( registers->USER[0] );
	}

 	void SWI_KRN_CountBits      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_CountBits( registers->USER[1] );
	}

 	void SWI_KRN_DoIO      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_DoIO( registers->USER[1], registers->USER[2] );
	}

 	void SWI_KRN_ffs      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_ffs( registers->USER[1] );
	}

 	void SWI_KRN_FindDevice      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_FindDevice( registers->USER[1] );
	}

 	void SWI_KRN_FindFolio      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_FindFolio( registers->USER[1] );
	}

 	void SWI_KRN_FindItem      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_FindItem( registers->USER[1], registers->USER[2] );
	}

    void SWI_KRN_FindItemVA            (ARMRegisters *registers, DMA *dma)
	{
	}

 	void SWI_KRN_FindLSB      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_FindLSB( registers->USER[1] );
	}

 	void SWI_KRN_FindMSB      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_FindMSB( registers->USER[1] );
	}

 	void SWI_KRN_FindMsgPort      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_FindMsgPort( registers->USER[1] );
	}

 	void SWI_KRN_FindNamedItem      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_FindNamedItem( registers->USER[1], registers->USER[2] );
	}

 	void SWI_KRN_FindNamedNode      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_FindNamedNode( registers->USER[1], registers->USER[2] );
	}

 	void SWI_KRN_FindNodeFromHead      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_FindNodeFromHead( registers->USER[1], registers->USER[2] );
	}

 	void SWI_KRN_FindNodeFromTail      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_FindNodeFromTail( registers->USER[1], registers->USER[2] );
	}

 	void SWI_KRN_FindSemaphore      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_FindSemaphore( registers->USER[1] );
	}

 	void SWI_KRN_FindTagArg      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_FindTagArg( registers->USER[1], registers->USER[2] );
	}

 	void SWI_KRN_FindTask      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_FindTask( registers->USER[1] );
	}

 	void SWI_KRN_FindVersionedItem      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_FindVersionedItem( registers->USER[1], registers->USER[2], registers->USER[3], registers->USER[4] );
	}

 	void SWI_KRN_FirstNode      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_FirstNode( registers->USER[1] );
	}

 	void SWI_KRN_GetBankBits      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_GetBankBits( registers->USER[1] );
	}

 	void SWI_KRN_GetCurrentSignals      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_GetCurrentSignals( );
	}

 	void SWI_KRN_GetFolioFunc      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_GetFolioFunc( registers->USER[1], registers->USER[2] );
	}

 	void SWI_KRN_GetMsg      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_GetMsg( registers->USER[1] );
	}

 	void SWI_KRN_GetNodeCount      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_GetNodeCount( registers->USER[1] );
	}

 	void SWI_KRN_GetNodePosFromHead      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_GetNodePosFromHead( registers->USER[1], registers->USER[2] );
	}

 	void SWI_KRN_GetNodePosFromTail      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_GetNodePosFromTail( registers->USER[1], registers->USER[2] );
	}

 	void SWI_KRN_GetSysErr      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_GetSysErr( registers->USER[1], registers->USER[2], registers->USER[3] );
	}

 	void SWI_KRN_GetTagArg      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_GetTagArg( registers->USER[1], registers->USER[2], registers->USER[3] );
	}

 	void SWI_KRN_GetTaskSignals      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_GetTaskSignals( registers->USER[1] );
	}


 	void SWI_KRN_GetThisMsg      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_GetThisMsg( registers->USER[1] );
	}

 	void SWI_KRN_InitList      (ARMRegisters *registers, DMA *dma)
	{
		KRN_InitList( registers->USER[0], registers->USER[1] );
	}

 	void SWI_KRN_IsEmptyList      (ARMRegisters *registers, DMA *dma)
	{
		KRN_IsEmptyList( registers->USER[0] );
	}

 	void SWI_KRN_IsItemOpened      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_IsItemOpened( registers->USER[1], registers->USER[2] );
	}

 	void SWI_KRN_IsListEmpty      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_IsListEmpty( registers->USER[1] );
	}

 	void SWI_KRN_IsNode      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_IsNode( registers->USER[1], registers->USER[2] );
	}

 	void SWI_KRN_IsNodeB      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_IsNodeB( registers->USER[1], registers->USER[2] );
	}

 	void SWI_KRN_LastNode      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_LastNode( registers->USER[1] );
	}

 	void SWI_KRN_LockItem      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_LockItem( registers->USER[1], registers->USER[2] );
	}

 	void SWI_KRN_LockSemaphore      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_LockSemaphore( registers->USER[1], registers->USER[2] );
	}

 	void SWI_KRN_LookupItem      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_LookupItem( registers->USER[1] );
	}

 	void SWI_KRN_MayGetChar      (ARMRegisters *registers, DMA *dma)
	{
		KRN_MayGetChar( registers->USER[0], registers->USER[2] );
	}

 	void SWI_KRN_NextNode      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_NextNode( registers->USER[1] );
	}

 	void SWI_KRN_NextTagArg      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_NextTagArg( registers->USER[1] );
	}

 	void SWI_KRN_PrevNode      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_PrevNode( registers->USER[1] );
	}

 	void SWI_KRN_PrintError      (ARMRegisters *registers, DMA *dma)
	{
		KRN_PrintError( registers->USER[0], registers->USER[1], registers->USER[2], registers->USER[3] );
	}

 	void SWI_KRN_PrintfSysErr      (ARMRegisters *registers, DMA *dma)
	{
		KRN_PrintfSysErr( registers->USER[0] );
	}

 	void SWI_KRN_ReadHardwareRandomNumber      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_ReadHardwareRandomNumber( );
	}

 	void SWI_KRN_ReplyMsg      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_ReplyMsg( registers->USER[1], registers->USER[2], registers->USER[3], registers->USER[4] );
	}

 	void SWI_KRN_ReplySmallMsg      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_ReplySmallMsg( registers->USER[1], registers->USER[2], registers->USER[3], registers->USER[4] );
	}

 	void SWI_KRN_SampleSystemTime      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_SampleSystemTime();
	}

 	void SWI_KRN_SampleSystemTimeTV      (ARMRegisters *registers, DMA *dma)
	{
		KRN_SampleSystemTimeTV( registers->USER[1] );
	}

 	void SWI_KRN_SendIO      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_SendIO( registers->USER[1], registers->USER[2] );
	}

 	void SWI_KRN_SendMsg      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_SendMsg( registers->USER[1], registers->USER[2], registers->USER[3], registers->USER[4] );
	}

 	void SWI_KRN_SendSignal      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_SendSignal( registers->USER[1], registers->USER[2] );
	}

 	void SWI_KRN_SendSmallMsg      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_SendSmallMsg( registers->USER[1], registers->USER[2], registers->USER[3], registers->USER[4] );
	}

 	void SWI_KRN_SetExitStatus      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_SetExitStatus( registers->USER[1] );
	}

 	void SWI_KRN_SetFunction      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_SetFunction( registers->USER[1], registers->USER[2], registers->USER[3], registers->USER[4] );
	}

 	void SWI_KRN_SetItemOwner      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_SetItemOwner( registers->USER[1], registers->USER[2] );
	}
	
 	void SWI_KRN_SetItemPri      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_SetItemPri( registers->USER[1], registers->USER[2] );
	}

 	void SWI_KRN_SetNodePri      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_SetNodePri( registers->USER[1], registers->USER[2] );
	}

 	void SWI_KRN_UnlockItem      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_UnlockItem( registers->USER[1] );
	}

 	void SWI_KRN_UnlockSemaphore      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_UnlockSemaphore( registers->USER[1] );
	}

    // Closing and Deleting Items

 	void SWI_KRN_AbortIO      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_AbortIO( registers->USER[1] );
	}
    
    void SWI_KRN_ClearCurrentSignals      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_ClearCurrentSignals( registers->USER[1] );
	}
    
 	void SWI_KRN_CloseItem      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_CloseItem( registers->USER[1] );
	}

 	void SWI_KRN_CloseNamedDevice      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_CloseNamedDevice( registers->USER[1] );
	}

 	void SWI_KRN_DeleteIOReq      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_DeleteIOReq( registers->USER[1] );
	}

 	void SWI_KRN_DeleteItem      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_DeleteItem( registers->USER[1] );
	}

 	void SWI_KRN_DeleteMsg      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_DeleteMsg( registers->USER[1] );
	}

 	void SWI_KRN_DeleteMsgPort      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_DeleteMsgPort( registers->USER[1] );
	}

 	void SWI_KRN_DeleteSemaphore      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_DeleteSemaphore( registers->USER[1] );
	}

 	void SWI_KRN_DeleteThread      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_DeleteThread( registers->USER[1] );
	}

 	void SWI_KRN_DumpNode      (ARMRegisters *registers, DMA *dma)
	{
		KRN_DumpNode( registers->USER[0], registers->USER[1] );
	}

 	void SWI_KRN_DumpTagList      (ARMRegisters *registers, DMA *dma)
	{
		KRN_DumpTagList( registers->USER[0], registers->USER[1] );
	}

 	void SWI_KRN_FreeSignal      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_FreeSignal( registers->USER[1] );
	}

 	void SWI_KRN_RemHead      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_RemHead( registers->USER[1] );
	}

 	void SWI_KRN_RemNode      (ARMRegisters *registers, DMA *dma)
	{
		KRN_RemNode( registers->USER[0] );
	}

 	void SWI_KRN_RemTail      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_RemTail( registers->USER[1] );
	}

	// Waiting and Caution Items

 	void SWI_KRN_WaitIO      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_WaitIO( registers->USER[1] );
	}

 	void SWI_KRN_WaitPort      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_WaitPort( registers->USER[1], registers->USER[2] );
	}

 	void SWI_KRN_WaitSignal      (ARMRegisters *registers, DMA *dma)
	{
		registers->USER[0] = KRN_WaitSignal( registers->USER[1] );
	}

 	void SWI_KRN_Yield      (ARMRegisters *registers, DMA *dma)
	{
		KRN_Yield();
	}

}
}

#pragma warning (default : 4100)