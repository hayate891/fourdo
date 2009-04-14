#ifndef _INC_KERNEL
#define _INC_KERNEL

#include "ARMCPU.h"
#include "DMA.hpp"

#include "KernelTypes.hpp"

class Kernel
{
///////////////////////////////////
// Singleton implementation
public:
    static Kernel* getInstance();
protected:
    Kernel();
private:
    static Kernel* mInstance;

////////////////////////////////
// Other, normal implementation
public:
    void init( DMA* aDMA, ARMCPU* aCPU );
    KernelStruct*  getData();

protected:
    DMA*           mDMA;
    ARMCPU*        mCPU;

    KernelStruct*  mKernelStruct;
};

#endif