#ifndef _INC_KERNEL
#define _INC_KERNEL

#include "ARMCPU.h"
#include "DMAController.h"

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
    void init( DMAController* aDMA, ARMCPU* aCPU );
    KernelStruct*  getData();

protected:
    DMAController* mDMA;
    ARMCPU*        mCPU;

    KernelStruct*  mKernelStruct;
};

#endif