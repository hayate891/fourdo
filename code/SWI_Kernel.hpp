#ifndef _INC_SWI_KERNEL
#define _INC_SWI_KERNEL

#include "types.h"
#include <cstdio>

#include "KernelTypes.hpp"
#include "DMA.hpp"

///////////////////////////////////////////////////////////
// GENERAL

// kprintf
void KRN_kprintf(const char *format, int argc, void **argv);

///////////////////////////////////////////////////////////
// MEMORY

// Allocation
MemPtr KRN_malloc( int32 size );
MemPtr KRN_AllocMem( int32 s, uint32 t );//MemPtr KRN_AllocMem( int32 s, uint32 t );
MemPtr KRN_AllocMemBlocks( int32 size, uint32 typebits );
MemPtr KRN_AllocMemFromMemList( MemListPtr ml, int32 size, uint32 memflags);
MemPtr KRN_AllocMemFromMemLists( ListPtr l, int32 size, uint32 memflags);
MemListPtr KRN_AllocMemList( const MemPtr p, CharPtr name );

// Deletion
void KRN_free( MemPtr p );
void KRN_FreeMem( MemPtr p, int32 size );
void KRN_FreeMemList( MemListPtr ml );
void KRN_FreeMemToMemList( MemListPtr ml, MemPtr p, int32 size );
void KRN_FreeMemToMemLists( MemListPtr l, MemPtr p, int32 size );
int32 KRN_ScavengeMem( void );

// Memory info
void   KRN_AvailMem(MemInfoPtr minfo, uint32 memflags);
Err    KRN_ControlMem( MemPtr p, int32 size, int32 cmd, Item task );
int32  KRN_GetMemAllocAlignment( uint32 memflags );
int32  KRN_GetMemTrackSize( const MemPtr p );
uint32 KRN_GetMemType( const MemPtr p );
int32  KRN_GetPageSize( uint32 memflags );
bool   KRN_IsMemReadable( const MemPtr p, int32 size );
bool   KRN_IsMemWritable( const MemPtr p, int32 size );

// Memory debugging
Err KRN_DumpMemDebug(const TagArgPtr args);
Err KRN_CreateMemDebug(uint32 controlFlags, const TagArgPtr args);
Err KRN_DeleteMemDebug(void);
Err KRN_SanityCheckMemDebug(const TagArgPtr args);

///////////////////////////////////////////////////////////
// ITEMS

// Creating Items
void KRN_AddHead( ListPtr l, NodePtr n );
void KRN_AddTail( ListPtr l, NodePtr n );
Item KRN_CreateBufferedMsg( const CharPtr name, uint8 pri, Item mp, uint32 datasize );
Item KRN_CreateIOReq( const CharPtr name, uint8 pri, Item dev, Item mp );
Item KRN_CreateItem( int32 ct, TagArgPtr p );
Item KRN_CreateItemVA( int32 ct, uint32 tags, ... );
Item KRN_CreateMsg( const CharPtr name, uint8 pri, Item mp );
Item KRN_CreateMsgPort( const CharPtr name, uint8 pri, uint32 signal );
Item KRN_CreateSemaphore( const CharPtr name, uint8 pri );
Item KRN_CreateSizedItem( int32 ctype, TagArgPtr p,int32 size );
Item KRN_CreateSmallMsg( const CharPtr name, uint8 pri, Item mp );
Item KRN_CreateThread( const CharPtr name, uint8 pri, FunctionPtr code, int32 stacksize );
Item KRN_CreateUniqueMsgPort( const CharPtr name, uint8 pri, uint32 signal );
Item KRN_CreateUniqueSemaphore( const CharPtr name, uint8 pri );
void KRN_InsertNodeAfter(NodePtr oldNode, NodePtr newNode);
void KRN_InsertNodeBefore(NodePtr oldNode, NodePtr newNode);
void KRN_InsertNodeFromHead( ListPtr l, NodePtr n );
void KRN_InsertNodeFromTail( ListPtr l, NodePtr n );
int32 KRN_MkNodeID( uint8 a , uint8 b );
void KRN_UniversalInsertNode( ListPtr l, NodePtr n, FuncPtr func );

// Opening Items
Item KRN_FindAndOpenDevice( const CharPtr name );
Item KRN_FindAndOpenFolio( const CharPtr name );
Item KRN_FindAndOpenItem( int32 cType, TagArgPtr tp );
Item KRN_FindAndOpenItemVA( int32 cType, uint32 tags, ... );
Item KRN_FindAndOpenNamedItem( int32 ctype, const CharPtr name );
Item KRN_OpenItem( Item FoundItem, MemPtr args );
Item KRN_OpenNamedDevice( const CharPtr name, uint32 a );

// Managing Items
int32 KRN_AllocSignal( uint32 sigMask );
//void KRN_CallFolio( FolioPtr folio, int32 func, args );
//void KRN_CallFolioRet( FolioPtr folio, int32 func, args, ret, cast );
int32 KRN_CheckIO( Item ior );
CheckPtr KRN_CheckItem( Item i, uint8 ftype, uint8 ntype );
void KRN_CompleteIO( IOReqPtr ior );
int KRN_CountBits(uint32 mask);
Err KRN_DoIO( Item ior, IOInfoPtr ioiP );
int KRN_ffs( uint32 mask );
Item KRN_FindDevice( const CharPtr name );
Item KRN_FindFolio( const CharPtr name );
Item KRN_FindItem( int32 cType, TagArgPtr tp );
Item KRN_FindItemVA( int32 cType, uint32 tags, ...);
int KRN_FindLSB( uint32 mask );
int KRN_FindMSB( uint32 bits );
Item KRN_FindMsgPort( const CharPtr name );
Item KRN_FindNamedItem( int32 ctype, const CharPtr name );
NodePtr KRN_FindNamedNode( ListPtr l, CharPtr name );
NodePtr KRN_FindNodeFromHead(ListPtr l, uint32 position);
NodePtr KRN_FindNodeFromTail(ListPtr l, uint32 position);
Item KRN_FindSemaphore( const CharPtr name );
TagArgPtr KRN_FindTagArg( TagArgPtr tagList, uint32 tag );
Item KRN_FindTask( const CharPtr name );
Item KRN_FindVersionedItem( int32 ctype, const CharPtr name, uint8 vers, uint8 rev );
NodePtr KRN_FirstNode( ListPtr l );
uint32 KRN_GetBankBits( MemPtr a );
int32 KRN_GetCurrentSignals( );
FuncPtr KRN_GetFolioFunc( FolioPtr folio, int32 func);
Item KRN_GetMsg( Item mp );
uint32 KRN_GetNodeCount(ListPtr l);
int32 KRN_GetNodePosFromHead(ListPtr l, NodePtr n);
int32 KRN_GetNodePosFromTail(ListPtr l, NodePtr n);
int32 KRN_GetSysErr( CharPtr buff, int32 buffsize, Err err );
TagArgPtr KRN_GetTagArg (TagArgPtr tagList, uint32 tag, TagArgPtr defaultValue);
int32 KRN_GetTaskSignals( TaskPtr t );
Item KRN_GetThisMsg( Item message );
void KRN_InitList( ListPtr l, CharPtr name );
bool KRN_IsEmptyList( ListPtr l );
Err KRN_IsItemOpened( Item task, Item i );
bool KRN_IsListEmpty( ListPtr l );
bool KRN_IsNode( ListPtr l, NodePtr n );
bool KRN_IsNodeB( ListPtr l, NodePtr n );
NodePtr KRN_LastNode( ListPtr l );
int32 KRN_LockItem( Item s, uint32 flags );
int32 KRN_LockSemaphore( Item s, uint32 flags );
ItemPtr KRN_LookupItem( Item i );
int KRN_MayGetChar( Item i,Item newOwner );
NodePtr KRN_NextNode( NodePtr n );
TagArgPtr KRN_NextTagArg( TagArgPtrPtr tagList );
NodePtr KRN_PrevNode( NodePtr node );
void KRN_PrintError( CharPtr who, CharPtr what, CharPtr whom, Err err );
void KRN_PrintfSysErr( Err err );
uint32 KRN_ReadHardwareRandomNumber();
Err KRN_ReplyMsg( Item msg, int32 result, MsgPtr dataptr,int32 datasize );
Err KRN_ReplySmallMsg( Item msg, int32 result, uint32 val1, uint32 val2);
uint32 KRN_SampleSystemTime( );
void KRN_SampleSystemTimeTV( TimeValPtr time );
Err KRN_SendIO( Item ior, IOInfoPtr ioiP );
Err KRN_SendMsg( Item mp, Item msg, MsgPtr dataptr, int32 datasize );
Err KRN_SendSignal( Item task, uint32 sigMask );
Err KRN_SendSmallMsg( Item mp, Item msg, uint32 val1, uint32 val2 );
Err KRN_SetExitStatus( int32 status );
MemPtr KRN_SetFunction( Item, int32 vnum, int32 vtype, SetFuncPtr newfunc );
Err KRN_SetItemOwner( Item i, Item newOwner );
int32 KRN_SetItemPri( Item i, uint8 newpri );
uint8 KRN_SetNodePri( NodePtr n, uint8 newpri );
Err KRN_UnlockItem( Item s );
Err KRN_UnlockSemaphore( Item s );

// Closing and Deleting Items
Err KRN_AbortIO( Item ior );
Err KRN_ClearCurrentSignals( int32 sigMask );
Err KRN_CloseItem( Item i );
Err KRN_CloseNamedDevice(Item devItem);
Err KRN_DeleteIOReq( Item item );
Err KRN_DeleteItem( Item i );
Err KRN_DeleteMsg( Item it );
Err KRN_DeleteMsgPort( Item it );
Err KRN_DeleteSemaphore( Item s );
Err KRN_DeleteThread( Item x );
void KRN_DumpNode (NodePtr node, CharPtr banner);
void KRN_DumpTagList (TagArgPtr tagList, CharPtr desc);
Err KRN_FreeSignal( uint32 sigMask );
NodePtr KRN_RemHead( ListPtr l );
void KRN_RemNode( NodePtr n );
NodePtr KRN_RemTail( ListPtr l );

// Waiting and Caution Items
Err KRN_WaitIO( Item ior );
Item KRN_WaitPort( Item mp, Item msg );
int32 KRN_WaitSignal( uint32 sigMask );
void KRN_Yield();

#endif