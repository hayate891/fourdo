#include "DMA.hpp"

#include "BitMath.h"
#include <cstdio>

// I found this out of the MESS project's 3do file. 
// It matches mappings found in the FreeDO project.
/*
AM_RANGE(0x00000000, 0x001FFFFF) AM_RAMBANK(1) AM_BASE(&dram)              // DRAM
AM_RANGE(0x00200000, 0x002FFFFF) AM_RAM	AM_BASE(&vram)                     // VRAM
AM_RANGE(0x03000000, 0x030FFFFF) AM_ROMBANK(2)                             // BIOS
AM_RANGE(0x03140000, 0x0315FFFF) AM_READWRITE(nvarea_r, nvarea_w)          // NVRAM
AM_RANGE(0x03180000, 0x031FFFFF) AM_READWRITE(unk_318_r, unk_318_w)        // ????
AM_RANGE(0x03200000, 0x032FFFFF) AM_READWRITE(vram_sport_r, vram_sport_w)  // special vram access1
AM_RANGE(0x03300000, 0x033FFFFF) AM_READWRITE(madam_r, madam_w)            // address decoder
AM_RANGE(0x03400000, 0x034FFFFF) AM_READWRITE(clio_r, clio_w)              // io controller
*/

////////////////////////////////
// Singleton implementation
DMA* DMA::mInstance = NULL;
DMA* DMA::getInstance() 
{
    if (mInstance == NULL) 
        mInstance = new DMA();
    return mInstance;
}

////////////////////////////////
// Other, normal implementation
DMA::DMA()
{
    mInitialized = false;
}

DMA::~DMA()
{
	delete mDRAM;
	delete mVRAM;
	delete mBIOS;
}

void DMA::init()
{
    if( mInitialized )
    {
        delete mDRAM;
        delete mVRAM;
        delete mBIOS;
    }
    
    mInitialized = true;
	
	mDRAM = new uchar[ DRAM_SIZE ];
	mVRAM = new uchar[ VRAM_SIZE ];
	mBIOS = new uchar[ BIOS_SIZE ];

	memset( mDRAM, 0, DRAM_SIZE );
	memset( mVRAM, 0, VRAM_SIZE );
	memset( mBIOS, 0, BIOS_SIZE );
}

uchar DMA::getByte(uint address)
{
	if   ( ( address & 0xFFE00000) == 0x00000000 )
	{
		// DRAM
		return mDRAM[ address ^ 0x3 ];
	}
	else if( address >= 0x00200000 && address <= 0x002FFFFF )
	{
		// VRAM
		address -= 0x00200000;
		return mVRAM[ address ];
	}
	else if( address >= 0x03000000 && address <= 0x030FFFFF )
	{
		// BIOS
		address -= 0x03000000;
		return mBIOS[ address ];
	}
	else if( address >= 0x03140000 && address <= 0x0315FFFF )
	{
		// NVRAM
	}
	else if( address >= 0x03180000 && address <= 0x031FFFFF )
	{
		// ????
	}
	else if( address >= 0x03200000 && address <= 0x032FFFFF )
	{
		// Special VRAM Access1
	}
	else if( address >= 0x03300000 && address <= 0x033FFFFF )
	{
		// Address Decoder
	}
	else if( address >= 0x03400000 && address <= 0x034FFFFF )
	{
		// IO Controller
	}
	
	return 0;
}

uint DMA::getWord( uint address )
{
	if   ( ( address & 0xFFE00000) == 0x00000000 )
	{
		// DRAM
		address = WordAlign( address );
		return ( *((uint32*)(&mDRAM[address])) );
	}
	else if( address >= 0x00200000 && address <= 0x002FFFFF )
	{
		// VRAM
		address -= 0x00200000;
		address -= address % 4;
		return ( mVRAM[ address ] << 24 ) + ( mVRAM[ address + 1 ] << 16 ) + ( mVRAM[ address + 2 ] << 8 ) + mVRAM[ address + 3 ];
	}
	else if( address >= 0x03000000 && address <= 0x030FFFFF )
	{
		// BIOS
		address -= 0x03000000;
		address -= address % 4;
		return ( mBIOS[ address ] << 24 ) + ( mBIOS[ address + 1 ] << 16 ) + ( mBIOS[ address + 2 ] << 8 ) + mBIOS[ address + 3 ];
	}
	else if( address >= 0x03140000 && address <= 0x0315FFFF )
	{
		// NVRAM
	}
	else if( address >= 0x03180000 && address <= 0x031FFFFF )
	{
		// ????
	}
	else if( address >= 0x03200000 && address <= 0x032FFFFF )
	{
		// Special VRAM Access1
	}
	else if( address >= 0x03300000 && address <= 0x033FFFFF )
	{
		// Address Decoder
	}
	else if( address >= 0x03400000 && address <= 0x034FFFFF )
	{
		// IO Controller
	}
	
	return 0;
}

void DMA::setByte( uint address, uchar value )
{
	if   ( ( address & 0xFFE00000 ) == 0x00000000 )
	{
		// DRAM
		mDRAM[ address ^ 0x3 ] = value;
	}
	else if( address >= 0x00200000 && address <= 0x002FFFFF )
	{
		// VRAM
		address -= 0x00200000;
		mVRAM[ address ] = value;
	}
	else if( address >= 0x03000000 && address <= 0x030FFFFF )
	{
		// BIOS
		address -= 0x03000000;
		mBIOS[ address ] = value;
	}
	else if( address >= 0x03140000 && address <= 0x0315FFFF )
	{
		// NVRAM
	}
	else if( address >= 0x03180000 && address <= 0x031FFFFF )
	{
		// ????
	}
	else if( address >= 0x03200000 && address <= 0x032FFFFF )
	{
		// Special VRAM Access1
	}
	else if( address >= 0x03300000 && address <= 0x033FFFFF )
	{
		// Address Decoder
	}
	else if( address >= 0x03400000 && address <= 0x034FFFFF )
	{
		// IO Controller
	}
}

void DMA::setWord( uint address, uint value )
{
	if    ( ( address & 0xFFE00000 ) == 0x00000000 )
	{
		// DRAM
		address = WordAlign( address );
		*((uint32*)(&mDRAM[address])) = value;
	}
	else if( address >= 0x00200000 && address <= 0x002FFFFF )
	{
		// VRAM
		address -= 0x00200000;
		address -= address % 4;
		mVRAM[ address     ] = ( uchar )( ( value & 0xFF000000 ) >> 24 );
		mVRAM[ address + 1 ] = ( uchar )( ( value & 0x00FF0000 ) >> 16 );
		mVRAM[ address + 2 ] = ( uchar )( ( value & 0x0000FF00 ) >> 8  );
		mVRAM[ address + 3 ] = ( uchar )(   value & 0x000000FF );
	}
	else if( address >= 0x03000000 && address <= 0x030FFFFF )
	{
		// BIOS
		address -= 0x03000000;
		address -= address % 4;
		mBIOS[ address     ] = ( uchar )( ( value & 0xFF000000 ) >> 24 );
		mBIOS[ address + 1 ] = ( uchar )( ( value & 0x00FF0000 ) >> 16 );
		mBIOS[ address + 2 ] = ( uchar )( ( value & 0x0000FF00 ) >> 8  );
		mBIOS[ address + 3 ] = ( uchar )(   value & 0x000000FF );
	}
	else if( address >= 0x03140000 && address <= 0x0315FFFF )
	{
		// NVRAM
	}
	else if( address >= 0x03180000 && address <= 0x031FFFFF )
	{
		// ????
	}
	else if( address >= 0x03200000 && address <= 0x032FFFFF )
	{
		// Special VRAM Access1
	}
	else if( address >= 0x03300000 && address <= 0x033FFFFF )
	{
		// Address Decoder
	}
	else if( address >= 0x03400000 && address <= 0x034FFFFF )
	{
		// IO Controller
	}
}

uchar* DMA::getPtr( uint address )
{
	if( address >= 0x03000000 && address <= 0x030FFFFF )
	{
		// BIOS
		return &( mBIOS[ address - 0x03000000 ] );
	}
	else if( address >= 0x00200000 && address <= 0x002FFFFF )
	{
		// VRAM
		return &( mVRAM[ address - 0x00200000 ] );
	}
	else
	{
		// just use DRAM
		return &( mDRAM[ address ] );
	}
}
