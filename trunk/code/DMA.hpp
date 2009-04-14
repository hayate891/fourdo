#ifndef _INC_DMA
#define _INC_DMA

#include "types.h"

//////////////////////////////////////////////////

#define DRAM_SIZE 0x200000 // 2 megs of DRAM
#define VRAM_SIZE 0x100000 // 1 megs of VRAM
#define BIOS_SIZE 0x100000 // 1 meg  of BIOS
#define VRAM_SIZE 0x100000 // 1 meg  of BIOS

class DMA
{
///////////////////////////////////
// Singleton implementation
public:
    static DMA* getInstance();
protected:
    DMA();
    ~DMA();
private:
    static DMA* mInstance;

////////////////////////////////
// Other, normal implementation
public:
	uchar  getByte( uint address );
	void   setByte( uint address, uchar value );

	uint   getWord( uint address );
	void   setWord( uint address, uint  value );
	uchar* getPtr( uint address );
	
	void   init();

protected:
	uchar* mDRAM;
	uchar* mVRAM;
	uchar* mBIOS;
	
	bool   mInitialized;
};

#endif //_INC_DMA
