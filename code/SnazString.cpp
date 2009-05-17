#include "SnazString.hpp"

#include "DMA.hpp"
#include "BitMath.h"

SnazString::SnazString()
{
}

SnazString::SnazString(uint32 aPtr)
{
    mPtr = aPtr;
}

void SnazString::init(uint32 aPtr)
{
    mPtr = aPtr;
}

std::string SnazString::get()
{
    DMA*  dma = DMA::getInstance();
    char* ptr;
    
    ptr = (char*)dma->getPtr(mPtr);
    return (std::string)(ReadSwappedString(ptr));
}

uint32 SnazString::ptr()
{
    return mPtr;
}