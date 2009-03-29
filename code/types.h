// NOTE: I don't understand this, but when I switched to Visual C++ 2005 (from 6.0),
//       I had to take the typedefs out of the inclusion check.
typedef unsigned int   uint;
typedef unsigned char  uchar;

typedef unsigned char  uint8;
typedef unsigned short uint16;
typedef unsigned int   uint32;

typedef signed char    int8;
typedef signed short   int16;
typedef signed int     int32;

#ifndef _INC_TYPES
#define _INC_TYPES

#endif //_INC_TYPES