#include "SWI_Kernel.hpp"

#pragma warning (disable : 4100) // Disable damned "unused param" warnings.

// Creating Items
Item KRN_CreateBufferedMsg( const char *name, uint8 pri, Item mp, uint32 datasize )
{
	return NULL;
}

Item KRN_CreateIOReq( const char *name, uint8 pri, Item dev, Item mp )
{
	return NULL;
}

Item KRN_CreateItem( int32 ct, TagArg *p )
{
	return NULL;
}

Item KRN_CreateItemVA( int32 ct, uint32 tags, ... )
{
	return NULL;
}

Item KRN_CreateMsg( const char *name, uint8 pri, Item mp )
{
	return NULL;
}

Item KRN_CreateMsgPort( const char *name, uint8 pri, uint32 signal )
{
	return NULL;
}

Item KRN_CreateSemaphore( const char *name, uint8 pri )
{
	return NULL;
}

Item KRN_CreateSmallMsg( const char *name, uint8 pri, Item mp )
{
	return NULL;
}

Item KRN_CreateThread( const char *name, uint8 pri, void (*code) (),int32 stacksize )
{
	return NULL;
}

Item KRN_CreateUniqueMsgPort( const char *name, uint8 pri, uint32 signal )
{
	return NULL;
}

Item KRN_CreateUniqueSemaphore( const char *name, uint8 pri )
{
	return NULL;
}


// Opening Items
Item KRN_FindAndOpenDevice( const char *name )
{
	return NULL;
}

Item KRN_FindAndOpenFolio( const char *name )
{
	return NULL;
}

Item KRN_FindAndOpenItem( int32 cType, TagArg *tp )
{
	return NULL;
}

Item KRN_FindAndOpenItemVA( int32 cType, uint32 tags, ... )
{
	return NULL;
}

Item KRN_FindAndOpenNamedItem( int32 ctype, const char *name )
{
	return NULL;
}

Item KRN_OpenItem( Item FoundItem, void *args )
{
	return NULL;
}

Item KRN_OpenNamedDevice( const char *name, uint32 a )
{
	return NULL;
}


// Managing Items
void* KRN_CheckItem( Item i, uint8 ftype, uint8 ntype )
{
	return NULL;
}

Item KRN_FindDevice( const char *name )
{
	return NULL;
}

Item KRN_FindFolio( const char *name )
{
	return NULL;
}

Item KRN_FindItem( int32 cType, TagArg *tp )
{
	return NULL;
}

Item KRN_FindItemVA( int32 cType, uint32 tags, ...)
{
	return NULL;
}

Item KRN_FindMsgPort( const char *name )
{
	return NULL;
}

Item KRN_FindNamedItem( int32 ctype, const char *name )
{
	return NULL;
}

Item KRN_FindSemaphore( const char *name )
{
	return NULL;
}

Item KRN_FindTask( const char *name )
{
	return NULL;
}

Item KRN_FindVersionedItem( int32 ctype, const char *name, uint8 vers, uint8 rev )
{
	return NULL;
}

void* KRN_LookupItem( Item i )
{
	return NULL;
}

Err KRN_SetItemOwner( Item i, Item newOwner )
{
	return NULL;
}

int32 KRN_SetItemPri( Item i, uint8 newpri )
{
	return NULL;
}


// Closing and Deleting Items
Err KRN_CloseItem( Item i )
{
	return NULL;
}

Err KRN_CloseNamedDevice( Item devItem )
{
	return NULL;
}

Err KRN_DeleteIOReq( Item item )
{
	return NULL;
}

Err KRN_DeleteItem( Item i )
{
	return NULL;
}

Err KRN_DeleteMsg( Item it )
{
	return NULL;
}

Err KRN_DeleteMsgPort( Item it )
{
	return NULL;
}

Err KRN_DeleteSemaphore( Item s )
{
	return NULL;
}

Err KRN_DeleteThread( Item x )
{
	return NULL;
}

#pragma warning (default : 4100)