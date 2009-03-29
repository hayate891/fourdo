#include "SWI_Kernel.hpp"

#pragma warning (disable : 4100) // Disable damned "unused param" warnings.

// Allocation
MemPtr KRN_malloc( int32 size )
{
	return NULL;
}

MemPtr KRN_AllocMem( int32 s, uint32 t )
{
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

#pragma warning (default : 4100)