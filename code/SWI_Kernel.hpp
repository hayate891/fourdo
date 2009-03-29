#ifndef _INC_SWI_KERNEL
#define _INC_SWI_KERNEL

#include "types.h"
#include <cstdio>

typedef uint32 Item;
typedef uint32 Err;

typedef uint32 MemPtr;
typedef uint32 MemListPtr;
typedef uint32 MemInfoPtr;
typedef uint32 ListPtr;
typedef uint32 TagArgPtr;

typedef uint32 CharPtr;

typedef void List;
typedef void MemList;
typedef void MemInfo;
typedef void TagArg;

///////////////////////////////////////////////////////////
// MEMORY

// Allocation
MemPtr KRN_malloc( int32 size );
MemPtr KRN_AllocMem( int32 s, uint32 t );
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
Item KRN_CreateBufferedMsg( const char *name, uint8 pri, Item mp, uint32 datasize );
Item KRN_CreateIOReq( const char *name, uint8 pri, Item dev, Item mp );
Item KRN_CreateItem( int32 ct, TagArg *p );
Item KRN_CreateItemVA( int32 ct, uint32 tags, ... );
Item KRN_CreateMsg( const char *name, uint8 pri, Item mp );
Item KRN_CreateMsgPort( const char *name, uint8 pri, uint32 signal );
Item KRN_CreateSemaphore( const char *name, uint8 pri );
Item KRN_CreateSmallMsg( const char *name, uint8 pri, Item mp );
Item KRN_CreateThread( const char *name, uint8 pri, void (*code) (),int32 stacksize );
Item KRN_CreateUniqueMsgPort( const char *name, uint8 pri, uint32 signal );
Item KRN_CreateUniqueSemaphore( const char *name, uint8 pri );

// Opening Items
Item KRN_FindAndOpenDevice( const char *name );
Item KRN_FindAndOpenFolio( const char *name );
Item KRN_FindAndOpenItem( int32 cType, TagArg *tp );
Item KRN_FindAndOpenItemVA( int32 cType, uint32 tags, ... );
Item KRN_FindAndOpenNamedItem( int32 ctype, const char *name );
Item KRN_OpenItem( Item FoundItem, void *args );
Item KRN_OpenNamedDevice( const char *name, uint32 a );

// Managing Items
void* KRN_CheckItem( Item i, uint8 ftype, uint8 ntype );
Item KRN_FindDevice( const char *name );
Item KRN_FindFolio( const char *name );
Item KRN_FindItem( int32 cType, TagArg *tp );
Item KRN_FindItemVA( int32 cType, uint32 tags, ...);
Item KRN_FindMsgPort( const char *name );
Item KRN_FindNamedItem( int32 ctype, const char *name );
Item KRN_FindSemaphore( const char *name );
Item KRN_FindTask( const char *name );
Item KRN_FindVersionedItem( int32 ctype, const char *name, uint8 vers, uint8 rev );
void* KRN_LookupItem( Item i );
Err KRN_SetItemOwner( Item i, Item newOwner );
int32 KRN_SetItemPri( Item i, uint8 newpri );

// Closing and Deleting Items
Err KRN_CloseItem( Item i );
Err KRN_CloseNamedDevice(Item devItem);
Err KRN_DeleteIOReq( Item item );
Err KRN_DeleteItem( Item i );
Err KRN_DeleteMsg( Item it );
Err KRN_DeleteMsgPort( Item it );
Err KRN_DeleteSemaphore( Item s );
Err KRN_DeleteThread( Item x );

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
// JMK This is just here for reference
typedef struct ItemNode
{
    struct ItemNode *n_Next; /*pointer to next itemnode in */

                                /* list */
    struct ItemNode *n_Prev; /*pointer to previous itemnode in 

                                /* list */
    uint8 n_SubsysType;    /* what folio manages this node */
    uint8 n_Type;        /* what type of node for the folio */
    uint8 n_Priority;    /* queueing priority */
    uint8 n_Flags;        /* misc flags, see below */
    int32 n_Size;        /* total size of node including hdr */
    char *n_Name;        /* ptr to null terminated string or NULL*/
    uint8 n_Version;    /* version of of this itemnode */
    uint8 n_Revision;    /* revision of this itemnode */
    uint8 n_FolioFlags;    /* flags for this item's folio */
    uint8 n_ItemFlags;    /* additional system item flags */
    Item  n_Item;        /* ItemNumber for this data structure */
    Item  n_Owner;        /* creator, present owner, disposer */
    void *n_ReservedP;    /* Reserved pointer */
} ItemNode, *ItemNodeP;

#endif