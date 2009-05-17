#ifndef _SNAZSTRING_H_
#define _SNAZSTRING_H_

#include "types.h"
#include <string>

class SnazString
{
public:
    SnazString();
    SnazString(uint32 aPtr);
    
    void   init(uint32 aPtr);
    std::string get();
    uint32      ptr();

protected:
    uint32 mPtr;
};

#endif