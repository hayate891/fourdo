#include "Kernel.hpp"
#include "KernelFaker.hpp"
#include <string>

////////////////////////////////
// Singleton implementation
Kernel* Kernel::mInstance = NULL;
Kernel* Kernel::getInstance() 
{
    if (mInstance == NULL) 
        mInstance = new Kernel();
    return mInstance;
}


////////////////////////////////
// Other, normal implementation
Kernel::Kernel() 
{
}

void Kernel::init( DMA* aDMA, ARMCPU* aCPU )
{
    mDMA = aDMA;
    mCPU = aCPU;
    
    mKernelStruct = (KernelStruct*)(mDMA->getPtr(KernelFaker::getFakeKernelBase()));
}

KernelStruct* Kernel::getData()
{
    return mKernelStruct;
}