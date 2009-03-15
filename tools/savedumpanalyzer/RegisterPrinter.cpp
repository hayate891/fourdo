#include "RegisterPrinter.hpp"

#include <stdio.h>

void writeRegInfo( ARM_CoreState* reg )
{
	int x;
	char str[255];
	char hex[9];
	
	FILE * regFile;
	fopen_s( &regFile, "SDA_Registers.txt","w");
	
	if( regFile == NULL )
		return;
	
	////////////////////////////////////////
	// Output user mode registers
	
	fputs( "=============================================\n", regFile );
	fputs( "(User Mode Registers)   \n", regFile );
	fputs( "\n", regFile );
	
	for( x = 0; x < 16; x++ )
	{
		UintToHexString( reg->USER[x], hex );
		switch(x)
		{
		case 13:
			sprintf_s( str, "Reg_%02i(SP):  [%s]\n", x, hex );
			break;
		case 14:
			sprintf_s( str, "Reg_%02i(LR):  [%s]\n", x, hex );
			break;
		case 15:
			sprintf_s( str, "Reg_%02i(PC):  [%s]\n", x, hex );
			break;
		default:
			sprintf_s( str, "Reg_%02i:      [%s]\n", x, hex );
			break;
		
		}
		fputs( str, regFile );
	}
	
	fputs( "\n", regFile );
	
	UintToHexString( reg->CPSR, hex );
	sprintf_s( str, "CPSR:        [%s]\n", hex );
	fputs( str, regFile );
	
	fputs( "\n", regFile );
	fputs( "\n", regFile );
	
	////////////////////////////////////////
	// Output supervisor mode registers
	
	fputs( "=============================================\n", regFile );
	fputs( "(Supervisor Mode Registers)   \n", regFile );
	fputs( "\n", regFile );
	
	UintToHexString( reg->SVC[0], hex );
	sprintf_s( str, "R13_svc:     [%s]\n", hex );
	fputs( str, regFile );

	UintToHexString( reg->SVC[1], hex );
	sprintf_s( str, "R14_svc:     [%s]\n", hex );
	fputs( str, regFile );
	
	fputs( "\n", regFile );
	
	UintToHexString( reg->SPSR[3], hex );
	sprintf_s( str, "CPSR_svc:    [%s]\n", hex );
	fputs( str, regFile );	

	fputs( "\n", regFile );
	fputs( "\n", regFile );

	////////////////////////////////////////
	// Output abort mode registers
	
	fputs( "=============================================\n", regFile );
	fputs( "(Abort Mode Registers)   \n", regFile );
	fputs( "\n", regFile );
	
	UintToHexString( reg->ABT[0], hex );
	sprintf_s( str, "R13_abt:     [%s]\n", hex );
	fputs( str, regFile );

	UintToHexString( reg->ABT[1], hex );
	sprintf_s( str, "R14_abt:     [%s]\n", hex );
	fputs( str, regFile );
	
	fputs( "\n", regFile );
	
	UintToHexString( reg->SPSR[4], hex );
	sprintf_s( str, "CPSR_abt:    [%s]\n", hex );
	fputs( str, regFile );	

	fputs( "\n", regFile );
	fputs( "\n", regFile );

	////////////////////////////////////////
	// Output undefined mode registers
	
	fputs( "=============================================\n", regFile );
	fputs( "(Undefined Mode Registers)   \n", regFile );
	fputs( "\n", regFile );
	
	UintToHexString( reg->UND[0], hex );
	sprintf_s( str, "R13_und:     [%s]\n", hex );
	fputs( str, regFile );

	UintToHexString( reg->UND[1], hex );
	sprintf_s( str, "R14_und:     [%s]\n", hex );
	fputs( str, regFile );
	
	fputs( "\n", regFile );
	
	UintToHexString( reg->SPSR[5], hex );
	sprintf_s( str, "CPSR_und:    [%s]\n", hex );
	fputs( str, regFile );	

	fputs( "\n", regFile );
	fputs( "\n", regFile );

	////////////////////////////////////////
	// Output interrupt mode registers
	
	fputs( "=============================================\n", regFile );
	fputs( "(Interrupt Mode Registers)   \n", regFile );
	fputs( "\n", regFile );
	
	UintToHexString( reg->IRQ[0], hex );
	sprintf_s( str, "R13_irq:     [%s]\n", hex );
	fputs( str, regFile );

	UintToHexString( reg->IRQ[1], hex );
	sprintf_s( str, "R14_irq:     [%s]\n", hex );
	fputs( str, regFile );
	
	fputs( "\n", regFile );
	
	UintToHexString( reg->SPSR[2], hex );
	sprintf_s( str, "CPSR_irq:    [%s]\n", hex );
	fputs( str, regFile );	

	fputs( "\n", regFile );
	fputs( "\n", regFile );

	////////////////////////////////////////
	// Output fast-interrupt mode registers
	
	fputs( "=============================================\n", regFile );
	fputs( "(Fast-Interrupt Mode Registers)   \n", regFile );
	fputs( "\n", regFile );
	
	UintToHexString( reg->FIQ[0], hex );
	sprintf_s( str, "R13_fiq:     [%s]\n", hex );
	fputs( str, regFile );

	UintToHexString( reg->FIQ[1], hex );
	sprintf_s( str, "R14_fiq:     [%s]\n", hex );
	fputs( str, regFile );
	
	fputs( "\n", regFile );
	
	UintToHexString( reg->SPSR[1], hex );
	sprintf_s( str, "CPSR_fiq:    [%s]\n", hex );
	fputs( str, regFile );	
	
	fclose( regFile );
}


void UintToHexString (uint32 value, char* str)
{
	int		x;
	uint8	temp;
   
	char hexval[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

	for( x = 7; x >= 0; x-- )
	{
		temp = (value >> (4 * x)) & 0xF;
		str[7-x] = hexval[temp];
	}
	str[8]=0;
}