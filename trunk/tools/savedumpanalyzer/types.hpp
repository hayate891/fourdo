#ifndef _INC_TYPES
#define _INC_TYPES

typedef unsigned char uint8;
typedef unsigned int  uint32;

#pragma pack (push,1)
struct ARM_CoreState
{
uint8 *Ram;//4 bytes - does not matter
uint8 *Rom;//4 bytes - does not matter
uint8 *NVRam;//4 bytes - does not matter

uint32 USER[16];
uint32 CASH[7];
uint32 SVC[2];
uint32 ABT[2];
uint32 FIQ[7];
uint32 IRQ[2];
uint32 UND[2];
uint32 SPSR[6];
uint32 CPSR;

bool nFIQ; ///bool by 1 bytes
bool SecondROM;
bool MAS_Access_Exept;
};
#pragma pack (pop)

#endif