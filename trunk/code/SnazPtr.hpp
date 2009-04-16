#ifndef _SNAZPTR_H_
#define _SNAZPTR_H_

#include "types.h"
#include "DMA.hpp"

template <typename T>
class SnazPtr
{
public:
    SnazPtr();
    SnazPtr(uint32 aPtr);
    
    void   init(uint32 aPtr);
    T*     get();
    uint32 ptr();

protected:
    uint32 mPtr;
};

template <typename T>
SnazPtr<T>::SnazPtr()
{
}

template <typename T>
SnazPtr<T>::SnazPtr(uint32 aPtr)
{
    mPtr = aPtr;
}

template <typename T>
void SnazPtr<T>::init(uint32 aPtr)
{
    mPtr = aPtr;
}

template <typename T>
T* SnazPtr<T>::get()
{
    DMA* dma = DMA::getInstance();
    return (T*)(dma->getPtr(mPtr));
}

template <typename T>
uint32 SnazPtr<T>::ptr()
{
    return mPtr;
}

#endif