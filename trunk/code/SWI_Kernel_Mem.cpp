#include "SWI_Kernel.hpp"

#include "Kernel.hpp"
#include "KernelFaker.hpp"
#include "KernelTypes.hpp"
#include "BitMath.h"

#pragma warning (disable : 4100) // Disable damned "unused param" warnings.

typedef uint8 AllocType; enum
{
    ALLOC_ANY = 0,
    ALLOC_DRAM = 1,
    ALLOC_VRAM = 2
};

// Allocation
MemPtr KRN_malloc( int32 size )
{
	return NULL;
}

MemPtr KRN_AllocMem( int32 size, uint32 flags )
{
    Kernel* kernel = Kernel::getInstance();
    TaskStruct* currentTask;
    
    //////////////////////////////
    // Get allocation type...
    AllocType allocType;
    if( flags & MEMTYPE_VRAM )
    {
        allocType = ALLOC_VRAM;
        
        // TODO: Add VRAM bank selection here.
    }
    else if( flags & MEMTYPE_DRAM )
    {
        allocType = ALLOC_DRAM;
    }
    else
    {
        allocType = ALLOC_ANY;
    }
    
    //////////////////////////////
    // Extra options...
    
    if( flags & MEMTYPE_STARTPAGE )
    {
        // Allocate along 2k boundary
        if( flags & MEMTYPE_SYSTEMPAGESIZE )
        {
            // Allocate along a page boundary
        }
    }
    
    if( flags & MEMTYPE_MYPOOL )
    {
        // TODO: Don't allocate extra pages in this case.
    }
    
    if( flags & MEMTYPE_FROMTOP )
    {
        // TODO: Allocate from the top instead of the bottom.
    }
    
    
    //////////////////////////////
    // Allocate memory from available memory list.
    
    
    
    //////////////////////////////
    if( flags & MEMTYPE_FILL )
    {
        // TODO: Fill memory with value!
    }

	return NULL;
}

MemPtr KRN_AllocMemBlocks( int32 size, uint32 typebits )
{
	return NULL;
}

MemPtr KRN_AllocMemFromMemList( MemListPtr ml, int32 size, uint32 memflags)
{
	return NULL;
}

MemPtr KRN_AllocMemFromMemLists( ListPtr l, int32 size, uint32 memflags)
{
	return NULL;
}

MemListPtr KRN_AllocMemList( const MemPtr p, CharPtr name )
{
	return NULL;
}

// Deletion
void KRN_free( MemPtr p )
{
}

void KRN_FreeMem( MemPtr p, int32 size )
{
}

void KRN_FreeMemList( MemListPtr ml )
{
}

void KRN_FreeMemToMemList( MemListPtr ml, MemPtr p, int32 size )
{
}

void KRN_FreeMemToMemLists( MemListPtr l, MemPtr p, int32 size )
{
}

int32 KRN_ScavengeMem( void )
{
	return NULL;
}

// Memory info
void   KRN_AvailMem(MemInfoPtr minfo, uint32 memflags)
{
}

Err    KRN_ControlMem( MemPtr p, int32 size, int32 cmd, Item task )
{
	return NULL;
}

int32  KRN_GetMemAllocAlignment( uint32 memflags )
{
	return NULL;
}

int32  KRN_GetMemTrackSize( const MemPtr p )
{
	return NULL;
}

uint32 KRN_GetMemType( const MemPtr p )
{
	return NULL;
}

int32  KRN_GetPageSize( uint32 memflags )
{
	return NULL;
}

bool   KRN_IsMemReadable( const MemPtr p, int32 size )
{
	return NULL;
}

bool   KRN_IsMemWritable( const MemPtr p, int32 size )
{
	return NULL;
}

// Memory debugging
Err KRN_DumpMemDebug(const TagArgPtr args)
{
	return NULL;
}

Err KRN_CreateMemDebug(uint32 controlFlags, const TagArgPtr args)
{
	return NULL;
}

Err KRN_DeleteMemDebug(void)
{
	return NULL;
}

Err KRN_SanityCheckMemDebug(const TagArgPtr args)
{
	return NULL;
}

/////////////////////////
// Items

// Creating Items
void KRN_AddHead( ListPtr l, NodePtr n )
{
	return;
}

void KRN_AddTail( ListPtr l, NodePtr n )
{
    return;
}

Item KRN_CreateSizedItem( int32 ctype, TagArgPtr p,int32 size )
{
	return NULL;
}

void KRN_InsertNodeAfter(NodePtr oldNode, NodePtr newNode)
{
	return;
}

void KRN_InsertNodeBefore(NodePtr oldNode, NodePtr newNode)
{
	return;
}

void KRN_InsertNodeFromHead( ListPtr l, NodePtr n )
{
	return;
}

void KRN_InsertNodeFromTail( ListPtr l, NodePtr n )
{
	return;
}

int32 KRN_MkNodeID( uint8 a , uint8 b )
{
	return NULL;
}

void KRN_UniversalInsertNode( ListPtr l, NodePtr n, FuncPtr func )
{
	return;
}

// Opening Items

// Managing Items
int32 KRN_AllocSignal( uint32 sigMask )
{
	return NULL;
}

/*
void KRN_CallFolio( FolioPtr folio, int32 func, args )
{
	return;
}


void KRN_CallFolioRet( FolioPtr folio, int32 func, args, ret, cast )
{
	return;
}
*/

int32 KRN_CheckIO( Item ior )
{
	return NULL;
}


void KRN_CompleteIO( IOReqPtr ior )
{
	return;
}

int KRN_CountBits(uint32 mask)
{
	return NULL;
}

Err KRN_DoIO( Item ior, IOInfoPtr ioiP )
{
	return NULL;
}

int KRN_ffs( uint32 mask )
{
	return NULL;
}

int KRN_FindLSB( uint32 mask )
{
	return NULL;
}

int KRN_FindMSB( uint32 bits )
{
	return NULL;
}

NodePtr KRN_FindNamedNode( ListPtr l, CharPtr name )
{
	return NULL;
}

NodePtr KRN_FindNodeFromHead(ListPtr l, uint32 position)
{
	return NULL;
}

NodePtr KRN_FindNodeFromTail(ListPtr l, uint32 position)
{
	return NULL;
}

TagArgPtr KRN_FindTagArg( TagArgPtr tagList, uint32 tag )
{
	return NULL;
}

NodePtr KRN_FirstNode( ListPtr l )
{
	return NULL;
}

uint32 KRN_GetBankBits( MemPtr a )
{
	return NULL;
}

int32 KRN_GetCurrentSignals( )
{
	return NULL;
}

FuncPtr KRN_GetFolioFunc( FolioPtr folio, int32 func)
{
	return NULL;
}

Item KRN_GetMsg( Item mp )
{
	return NULL;
}

uint32 KRN_GetNodeCount(ListPtr l)
{
	return NULL;
}

int32 KRN_GetNodePosFromHead(ListPtr l, NodePtr n)
{
	return NULL;
}

int32 KRN_GetNodePosFromTail(ListPtr l, NodePtr n)
{
	return NULL;
}

int32 KRN_GetSysErr( CharPtr buff, int32 buffsize, Err err )
{
	return NULL;
}

TagArgPtr KRN_GetTagArg (TagArgPtr tagList, uint32 tag, TagArgPtr defaultValue)
{
	return NULL;
}

int32 KRN_GetTaskSignals( TaskPtr t )
{
	return NULL;
}

Item KRN_GetThisMsg( Item message )
{
	return NULL;
}

void KRN_InitList( ListPtr l, CharPtr name )
{
	return;
}

bool KRN_IsEmptyList( ListPtr l )
{
	return NULL;
}

Err KRN_IsItemOpened( Item task, Item i )
{
	return NULL;
}

bool KRN_IsListEmpty( ListPtr l )
{
	return NULL;
}

bool KRN_IsNode( ListPtr l, NodePtr n )
{
	return NULL;
}

bool KRN_IsNodeB( ListPtr l, NodePtr n )
{
	return NULL;
}

NodePtr KRN_LastNode( ListPtr l )
{
	return NULL;
}

int32 KRN_LockItem( Item s, uint32 flags )
{
	return NULL;
}

int32 KRN_LockSemaphore( Item s, uint32 flags )
{
	return NULL;
}

int KRN_MayGetChar( Item i,Item newOwner )
{
	return NULL;
}
NodePtr KRN_NextNode( NodePtr n )
{
	return NULL;
}

TagArgPtr KRN_NextTagArg( TagArgPtrPtr tagList )
{
	return NULL;
}

NodePtr KRN_PrevNode( NodePtr node )
{
	return NULL;
}

void KRN_PrintError( CharPtr who, CharPtr what, CharPtr whom, Err err )
{
	return;
}

void KRN_PrintfSysErr( Err err )
{
	return;
}

uint32 KRN_ReadHardwareRandomNumber( )
{
	return NULL;
}

Err KRN_ReplyMsg( Item msg, int32 result, MsgPtr dataptr,int32 datasize )
{
	return NULL;
}

Err KRN_ReplySmallMsg( Item msg, int32 result, uint32 val1, uint32 val2)
{
	return NULL;
}

uint32 KRN_SampleSystemTime( )
{
	return NULL;
}

void KRN_SampleSystemTimeTV( TimeValPtr time )
{
	return;
}

Err KRN_SendIO( Item ior, IOInfoPtr ioiP )
{
	return NULL;
}

Err KRN_SendMsg( Item mp, Item msg, MsgPtr dataptr, int32 datasize )
{
	return NULL;
}

Err KRN_SendSignal( Item task, uint32 sigMask )
{
	return NULL;
}

Err KRN_SendSmallMsg( Item mp, Item msg, uint32 val1, uint32 val2 )
{
	return NULL;
}

Err KRN_SetExitStatus( int32 status )
{
	return NULL;
}

MemPtr KRN_SetFunction( Item, int32 vnum, int32 vtype, SetFuncPtr newfunc )
{
	return NULL;
}

uint8 KRN_SetNodePri( NodePtr n, uint8 newpri )
{
	return NULL;
}

Err KRN_UnlockItem( Item s )
{
	return NULL;
}

Err KRN_UnlockSemaphore( Item s )
{
	return NULL;
}

// Closing and Deleting Items
Err KRN_AbortIO( Item ior )
{
	return NULL;
}

Err KRN_ClearCurrentSignals( int32 sigMask )
{
	return NULL;
}

void KRN_DumpNode (NodePtr node, CharPtr banner)
{
	return;
}

void KRN_DumpTagList (TagArgPtr tagList, CharPtr desc)
{
	return;
}

Err KRN_FreeSignal( uint32 sigMask )
{
	return NULL;
}

NodePtr KRN_RemHead( ListPtr l )
{
	return NULL;
}

void KRN_RemNode( NodePtr n )
{
	return;
}

NodePtr KRN_RemTail( ListPtr l )
{
	return NULL;
}

// Waiting and Caution Items
Err KRN_WaitIO( Item ior )
{
	return NULL;
}

Item KRN_WaitPort( Item mp, Item msg )
{
	return NULL;
}

int32 KRN_WaitSignal( uint32 sigMask )
{
	return NULL;
}

void KRN_Yield( )
{
	return;
}

#pragma warning (default : 4100)