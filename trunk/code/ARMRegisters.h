#ifndef _INC_ARMREGISTERS
#define _INC_ARMREGISTERS

#include "types.h"

// This structure stores all the registers
typedef struct
{
	uint32 USER[16];
	uint32 CASH[7];
    uint32 SVC[2];
    uint32 ABT[2];
    uint32 FIQ[7];
    uint32 IRQ[2];
    uint32 UND[2];
    uint32 SPSR[6];
	uint32 CPSR;
	
	bool nFIQ; //внешнее прерывание, устанавливается другими процессорами
	bool SecondROM;	//селектор ПЗУ (рум и шрифт)	
	bool MAS_Access_Exept;	//заведено для исключений доступа к памяти
	int CYCLES;	//здесь считаем циклы			
}ARMRegisters;

#endif