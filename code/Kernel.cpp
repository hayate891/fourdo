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
Kernel::Kernel() 
{
}

////////////////////////////////
// Other, normal implementation
void Kernel::init( DMAController* aDMA, ARMCPU* aCPU )
{
    mDMA = aDMA;
    mCPU = aCPU;
    
    mKernelStruct = (KernelStruct*)(mDMA->GetRAMPointer(KernelFaker::getFakeKernelBase()));
}

KernelStruct* Kernel::getData()
{
    return mKernelStruct;
}