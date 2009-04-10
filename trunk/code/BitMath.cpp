#include "BitMath.h"

uint SetBit (uint value, int bitNumber, bool )
{
   return SetBits (value, ((uint) pow ((double) 2, bitNumber)), 0xFFFFFFFF);
}
   
uint SetBits (uint value, uint setMask, uint setValue)
{
   return (value & ~setMask) + (setMask & setValue);
}

wxString CharToBitString (char value)
{
   const int  bits = 8;
   wxString   newRetVal;

   for (int x = 0; x < bits; x++)
   {
      newRetVal += wxString::Format(_T("%d"), ((value & ((uint) pow ((double) 2, bits - x - 1))) > 0) ? 1 : 0);
   }
   
   return newRetVal;
}

wxString UintToBitString (uint value)
{
   const int  bits = 32;
   wxString   newRetVal;

   for (int x = 0; x < bits; x++)
   {
      newRetVal += wxString::Format(_T("%d"), ((value & ((uint) pow ((double) 2, bits - x - 1))) > 0) ? 1 : 0);
   }

   return newRetVal;
}

wxString UintToHexString (uint value)
{
   int       value1;
   int       value2;
   int       value3;
   int       value4;
   wxString  ret;

   value1 = value >> 24;
   value2 = (value >> 16) & 0x000000FF;
   value3 = (value >> 8) & 0x000000FF;
   value4 = value & 0x000000FF;
   
   ret = wxDecToHex (value1);
   ret.append (wxDecToHex (value2));
   ret.append (wxDecToHex (value3));
   ret.append (wxDecToHex (value4));
   
   return ret;
}

std::string ReadSwappedString(char* buffer)
{
    char*       tempBuf;
    uint32      buffSize;
    std::string retval;
    uint32      nullchar;
    
    for( nullchar = 0; buffer[nullchar]!='\0'; nullchar++ );
    buffSize = WordAlign(nullchar)+4;
    tempBuf = new char[buffSize];
    memcpy( tempBuf, buffer, buffSize );
    
    ByteSwapMem( (uint32*)tempBuf, buffSize );
    retval = tempBuf;
    delete[] tempBuf;
    
    return retval;
}