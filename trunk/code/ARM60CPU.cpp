#include "ARM60CPU.h"

ARM60CPU::ARM60CPU ()
{
	REG = new ARM60Registers ();
	m_cycleCount = 0;
}

ARM60CPU::~ARM60CPU ()
{
	delete REG;
}

//The following is a condition table used to quickly check the condition
//field of the instruction against the CPSR.
//
//Credit given to Altmer for the idea.
//
//Most of these checks are simple, but some have multiple possible CPSR
//values that result success. To handle this, the table values below
//are shifted to the right by the current value in the CPSR, and the
//result is AND compared to the number "1". This allows multiple comparisons
//to be done in one operation.
//
// NOTE: The documentation specifies that the absence of a condition
//       code acts as though "always" had been specified, although
//       I don't see how this is possible.
const static uint condition_table[]=
	{
    0xf0f0, // EQ - Z set (equal)
    0x0f0f, // NE - Z clear (not equal)
    0xcccc, // CS - C set (unsigned higher or same)
    0x3333, // CC - C clear (unsigned lower)
    0xff00, // MI - N set (negative)
    0x00ff, // PL - N clear (positive or zero)
    0xaaaa, // VS - V set (overflow)
    0x5555, // VC - V clear (no overflow)
    0x0c0c, // HI - C set and Z clear (unsigned higher)
    0xf3f3, // LS - C clear or Z set (unsigned lower or same)
    0xaa55, // GE - N set and V set, or N clear and V clear (greater or equal)
    0x55aa, // LT - N set and V clear, or N clear and V set (less than)
    0x0a05, // GT - Z clear, and either N set and V set, or N clear and V clear (greater than)
    0xf5fa, // LE - Z set, or N set and V clear, or N clear and V set (less than or equal)
    0xffff, // AL - always
    0x0000  // NV - never
			// NOTE: The documentation specifies that the NV condition should
			//       not be used because it will be redefined in later ARM
			//       versions.
	};

///////////////////////////////////////////////////////////////////
// ExecuteCycles.
//////////////
// This will execute the specified number of cycles.
///////////////////////////////////////////////////////////////////
void ARM60CPU::ExecuteCycles( uint cycles )
{
	uint instruction;
	
	// Reset our cycle count to nothing.
	m_cycleCount = 0;
	
	// Cycle until we meet the specified quota.
	while (m_cycleCount < cycles)
	{
		// Read the next instruction.
		instruction = DMA->GetWord (*(REG->PC()->Value));

		// Move the PC.
		*(REG->PC()->Value) += 4;

		// Process it.
		this->ProcessInstruction( instruction );
	}
}

void ARM60CPU::DoSingleInstruction ()
{
	uint instruction;
	
	//Read next instruction
	instruction = DMA->GetWord (*(REG->PC()->Value));
	
	// Move the PC.
	*(REG->PC()->Value) += 4;

	// Process it.
	this->ProcessInstruction( instruction );
}

void ARM60CPU::ProcessInstruction (uint instruction)
{
	/////////////////////////////////////////
	// Check condition of the instruction to see if we should skip it.
	if
		(!
			(
				(
				condition_table[(((uint)instruction)>>28)]
				>> (*(REG->CPSR ()->Value)>>28)
				)
				& 1
			)
		)
	{
		#ifdef __FOURDODEBUG__
		wxLogMessage( "Condition check failed" );
		LastResult = _T( "Skip (Cond)" );
		#endif
		
		// Skipping an operation still takes cycles...
		m_cycleCount += ( SCYCLES * 2 ) + NCYCLES;
		return;	
	}
	
	/////////////////////
	// Some of these (especially those starting with 00
	// are rather cryptic, but I am sure they're a
	// deterministic way to figure the type of instruction.
	// So, don't toy with them thinking there's a faster way.
	//
	// Hopefully I did it right, or I'll have to do it
	// all over again...

	if      ((instruction & 0x0FC000F0) == 0x00000090)
	{
		// Multiply
		this->ProcessMultiply (instruction);
	}
	else if ((instruction & 0x0C000000) == 0x00000000)
	{
		// Starts with 00...
		if ((instruction & 0x01900000) == 0x01000000)
		{
			// Either MRS/MSR(PRS) or SWP
			if ((instruction & 0x02200080) == 0x00000080)
			{
				// SWP (Single Data Swap)
				this->ProcessSingleDataSwap (instruction);
			}
			else
			{
				// MRS/MSR (PRS)
				this->ProcessPSRTransfer (instruction);
			}
		}
		else
		{
			// Data Processing
			this->ProcessDataProcessing (instruction);
		}
	}
	else if ((instruction & 0x0C000000) == 0x04000000)
	{
		// Single Data Transfer (01)
		this->ProcessSingleDataTransfer (instruction);
	}
	else if ((instruction & 0x0E000000) == 0x06000000)
	{
		// Undefined (011)
		this->ProcessUndefined (instruction);
	}
	else if ((instruction & 0x0E000000) == 0x08000000)
	{
		// Block Data Transfer (100)
		this->ProcessBlockDataTransfer (instruction);
	}
	else if ((instruction & 0x0E000000) == 0x0A000000)
	{
		// Branch (101)
		this->ProcessBranch (instruction);
	}
	else if ((instruction & 0x0E000000) == 0x0C000000)
	{
		// Coproc Data Transfer (110)
		this->ProcessCoprocessorDataTransfers (instruction);
	}
	else if ((instruction & 0x0F000000) == 0x0F000000)
	{
		// Software Interrupt (1111)
		this->ProcessSoftwareInterrupt (instruction);
	}
	else if ((instruction & 0x0F000010) == 0x0E000010)
	{
		// Coproc Register Transfer (1110 ... 1)
		this->ProcessCoprocessorRegisterTransfers (instruction);
	}
	else if ((instruction & 0x0F000010) == 0x0E000000)
	{
		// Coproc Data Operation (1110 ... 0)
		this->ProcessCoprocessorDataOperations (instruction);
	}
	else
	{
		// Aw shit. Now what do we do?
	}
}

////////////////////////////////////////////////////////////
// Branch (B, BL)
////////////////////////////////////////////////////////////
void ARM60CPU::ProcessBranch (uint instruction)
{
#ifdef __FOURDODEBUG__
wxLogMessage( "Processed Branch" );
LastResult = _T( "Branch (Unused)" );
#endif
//////////////////////
//  3         2         1         0
// 10987654321098765432109876543210
// Cond   L
//     101 [________Offset________]
//////////////////////
int  offset;
bool isLink;

isLink = ( instruction & 0x01000000 ) == 0x01000000;

if( isLink)
	{
	// Save the PC value (+ 4)
	// This saves the address of the register following the branch instruction.
	*(REG->Reg (ARM60_R14)) = *(REG->PC ()->Value) + 4;
	}

// Offset is bit shifted left two, then sign extended to 32 bits.
offset = ( instruction & 0x00FFFFFF ) << 2;
if( ( offset & 0x02000000 ) > 0 )
	{
	offset |= 0xFC000000;
	}

// Add 4 bytes for prefetch.
offset += 4;

#ifdef __FOURDODEBUG__
// Give detailed info about this instruction.
LastResult = _T( "Branch" );
if( isLink )
	LastResult.Append( _T( "(with Link)" ) );
LastResult.Append( 
	wxString::Format( _T( " to %s" ), 
	UintToHexString( ( *( REG->PC()->Value ) ) + offset ) ) );
#endif

// Calculate # of cycles used.
m_cycleCount += ( SCYCLES * 2 ) + NCYCLES;

// Move the PC
( *( REG->PC()->Value ) ) += offset;
}

////////////////////////////////////////////////////////////
// Data Processing
////////////////////////////////////////////////////////////
void ARM60CPU::ProcessDataProcessing (uint instruction)
{
	#ifdef __FOURDODEBUG__
	wxLogMessage ("Processed Data Processing");
	LastResult = "Data Proc";
	#endif

	//////////////////////
	//  3         2         1         0
	// 10987654321098765432109876543210
	// Cond  I    S    [Rd]
	//     00 OpCd [Rn]    [ Operand 2]
	//////////////////////
	
	//////////////////////
	RegisterType op1Type;
	
	bool newCarry = false;
	bool setCond;
	bool writeResult = true; // NOTE: The assembler allegegly always sets S to false here.
	bool isLogicOp = true;
	uint op1;
	uint op2;
	uint result = 0;
	int  opCode;
	int  regDest;
	int  shift;

	opCode = (instruction & 0x01E00000) >> 21;
	setCond = (instruction & 0x00100000) > 0;
	regDest = (instruction & 0x0000F000) >> 12;
	
	////////////////////////////
	// Get first operand value.
	op1Type = (RegisterType) ((instruction & 0x000F0000) >> 16);
	op1 = *(REG->Reg (op1Type));
	
	// TODO: Is this prefetch affected by use of 
	//       a register with the shift of operand 2?
	if (op1Type == RegisterType::ARM60_PC)
		op1 += 4;
	
	////////////////////////////
	// Get second operand value.

	// Check Immediate Operand.
	if ((instruction & 0x02000000) > 0)
	{
		// Immediate - operand 2 is an immediate value.
		//  1 0 9 8 7 6 5 4 3 2 1 0
		// [Rotate][ImmediateValue]
		shift = ((instruction & 0x00000F00) >> 8) * 2; // NOTE: it rotates by double this amt.
		op2 = (instruction & 0x000000FF);

		// Rotate value.
		op2 = (op2 >> shift) | (op2 << (32 - shift));
	}
	else
	{
		// Not immediate - Operand 2 is a shifted register.
		op2 = ReadShiftedRegisterOperand (instruction, &newCarry);
	}
	
	///////////////////////
	// Handle each type of operation.

	switch (opCode)
	{
	case 0x0:
		// AND
		result = op1 & op2;
		break;
	
	case 0x1:
		// EOR
		result = op1 ^ op2;
		break;
	
	case 0x2:
		// SUB
		isLogicOp = false;
		result = DoAdd (op1, (~op2)+1, false, &newCarry);
		break;
	
	case 0x3:
		// RSB
		isLogicOp = false;
		result = DoAdd (op2, (~op1)+1, false, &newCarry);
		break;
	
	case 0x4:
		// ADD
		isLogicOp = false;
		result = DoAdd (op1, op2, false, &newCarry);
		break;
	
	case 0x5:
		// ADC
		isLogicOp = false;
		result = DoAdd (op1, op2, REG->CPSR ()->GetCarry (), &newCarry);
		break;
	
	case 0x6:
		// SBC
		isLogicOp = false;
		result = DoAdd (op1, (~op2)+1, REG->CPSR ()->GetCarry (), &newCarry);
		break;
	
	case 0x7:
		// RSC
		isLogicOp = false;
		result = DoAdd (op2, (~op1)+1, REG->CPSR ()->GetCarry (), &newCarry);
		break;
	
	case 0x8:
		// TST
		writeResult = false;
		result = op2 & op1;
		break;
	
	case 0x9:
		// TEQ
		writeResult = false;
		result = op2 ^ op1;
		break;
	
	case 0xA:
		// CMP
		writeResult = false;
		isLogicOp = false;
		result = DoAdd (op1, -(int)op2, REG->CPSR ()->GetCarry (), &newCarry);
		break;
	
	case 0xB:
		// CMN
		writeResult = false;
		isLogicOp = false;
		result = DoAdd (op1, -(int)op2, REG->CPSR ()->GetCarry (), &newCarry);
		break;
	
	case 0xC:
		// ORR
		result = op2 | op1;
		break;
	
	case 0xD:
		// MOV
		result = op2;
		break;

	case 0xE:
		// BIC
		result = op1 & (~op2);
		break;

	case 0xF:
		// MVN
		result = ~op2;
		break;

	}

	///////////////////////
	// Write result if necessary.
	if (writeResult)
	{
		if (regDest == (int) ARM60_PC)
		{
			// Writing to the PC, eh?
			
			// This takes some extra cycles.
			m_cycleCount += SCYCLES + NCYCLES;
			
			// Write it to the PC, but make sure it's a multiple of 4. (falls on word boundary)
			(*(REG->Reg ((RegisterType) regDest))) = result & 0xfffffffc;
		}
		else
		{
			// Normal write.
			(*(REG->Reg ((RegisterType) regDest))) = result;
		}
	}

	///////////////////////
	// Write condition flags if instructed to.

	if (setCond)
	{
		if (regDest == (int) ARM60_PC)
		{
			if (REG->CPSR ()->GetCPUMode() == CPUMODE_USR)
			{
				// Hm, this shouldn't happen.
				// TODO: Error?
			}
			else
			{
				// SPSR of current mode is written to CPSR;
				*(REG->Reg (ARM60_CPSR)) = *(REG->Reg (ARM60_SPSR));
			}
		}
		
		if (isLogicOp)
		{
			// Overflow bit is not changed in this scenario.
			REG->CPSR ()->SetCarry (newCarry);
			REG->CPSR ()->SetZero (result == 0);
			REG->CPSR ()->SetNegative ((result & 0x80000000) > 0);
		}
		else
		{
			// TODO: Double-check carry and overflow logic!!
			
			// Overflow bit is specific to two's compliment operations.
			// It will be set if the sign of the two operands differs
			// from the sign of the result.
			if ((op1 & 0x80000000) == (op2 & 0x80000000))
			{
				//////////////////////////////////////////////////////
				//////////////////////////////////////////////////////
				//////////////////////////////////////////////////////
				//TODO: overflow logic is confirmed to be BAD!!
				//////////////////////////////////////////////////////
				//////////////////////////////////////////////////////
				//////////////////////////////////////////////////////
				
				//REG->CPSR ()->SetOverflow (!
				//      ((result & 0x80000000) == (op1 & 0x80000000)));
			}
			else
			{
				// Overflow is impossible.
				REG->CPSR ()->SetOverflow (false);
			}
			REG->CPSR ()->SetCarry (newCarry);
			REG->CPSR ()->SetZero (result == 0);
			REG->CPSR ()->SetNegative ((result & 0x80000000) > 0);
		}
	}
	
	// These operations typically take just a single S cycle to run
	m_cycleCount += SCYCLES;
}

////////////////////////////////////////////////////////////
// PSR Transfer (MRS, MSR)
////////////////////////////////////////////////////////////
void ARM60CPU::ProcessPSRTransfer (uint instruction)
{
	#ifdef __FOURDODEBUG__
	LastResult = "PSR Trans";
	#endif

	//////////////////////////
	uint sourceVal;
	int  rotate;

	/////////////////
	// Determine operation
	switch (instruction & 0x003F0000)
	{
	case 0x000F0000:
		// MRS - transfer PSR contents to a register
		if ((instruction & 0x00400000) > 0)
		{
			// Source is SPSR
			*(REG->Reg ((RegisterType) ((instruction & 0x0000F000) > 12))) = 
				*(REG->Reg (ARM60_SPSR));
		}
		else
		{
			// Source is CPSR
			*(REG->Reg ((RegisterType) ((instruction & 0x0000F000) > 12))) = 
				*(REG->Reg (ARM60_CPSR));
		}
		break;

	case 0x00290000:
		// MSR - transfer register contents to PSR
		if ((instruction & 0x00400000) > 0)
		{
			// Destination is SPSR
			*(REG->Reg (ARM60_SPSR)) = 
				*(REG->Reg ((RegisterType) (instruction & 0x0000000F)));
		}
		else
		{
			// Destination is CPSR
			*(REG->Reg (ARM60_CPSR)) = 
				*(REG->Reg ((RegisterType) (instruction & 0x0000000F)));
		}
		break;

	case 0x00280000:
		// MSR - transfer register contents or immediate value to PSR (flag bits only)
		
		//////////////
		// Get source.
		if ((instruction & 0x02000000) > 0)
		{
			// Rotated immediate value.
			rotate = ((instruction & 0x00000F00) >> 8) * 2;
			sourceVal = (instruction & 0x000000FF);

			// Rotate value.
			sourceVal = (sourceVal >> rotate) | (sourceVal << (32 - rotate));

		}
		else
		{
			// Register value.
			sourceVal = *(REG->Reg ((RegisterType) (instruction & 0x0000000F)));
		}
		
		//////////////
		// Give to destination.
		if ((instruction & 0x00400000) > 0)
		{
			// Destination is SPSR
			*(REG->Reg (ARM60_SPSR)) = sourceVal;
		}
		else
		{
			// Destination is CPSR
			*(REG->Reg (ARM60_CPSR)) = sourceVal;
		}
		break;
	}
}

////////////////////////////////////////////////////////////
// Multiply (MUL, MLA)
////////////////////////////////////////////////////////////
void ARM60CPU::ProcessMultiply (uint instruction)
{
	#ifdef __FOURDODEBUG__
	LastResult = "Multiply";
	#endif
 
	//////////////////////
	//  3         2         1         0
	// 10987654321098765432109876543210
	// Cond      A [Rd]    [Rs]    [Rm]
	//     000000 S    [Rn]    1001 
	
	///////////////////////////

	uint result;
	RegisterType Rd;
	RegisterType Rn;
	RegisterType Rs;
	RegisterType Rm;
	bool accumulate;
	
	////////////////////////////
	Rd = (RegisterType) (instruction & 0x000F0000);
	Rn = (RegisterType) (instruction & 0x0000F000);
	Rs = (RegisterType) (instruction & 0x00000F00);
	Rm = (RegisterType) (instruction & 0x0000000F);
	accumulate = (instruction & 0x00200000) > 0;

	result = (*(REG->Reg (Rm))) * (*(REG->Reg (Rs)));

	if (Rm == Rd)
	{
		// Uh oh! Lowest-order register is also the destination!
		//
		// This is documented as a bad idea, since the source register is 
		// read multiple times throughout the multiplication.
		//
		// MUL returns 0
		// MLA returns a "meaningless result"

		result = 0;
	}
	
	// Optionally add Rn
	if (accumulate)
	{
		result += *(REG->Reg (Rn));
	}

	// Set the result to Rd
	*(REG->Reg ((RegisterType) (instruction & 0x000F0000))) = result;
	
	// Optionally set CPSR values.
	if ((instruction & 0x00100000) > 0)
	{
		// Set CPSR values.
		
		// NOTE: Overflow is unaffected.

		// NOTE: Carry is set to a "meaningless value".
		REG->CPSR ()->SetCarry (0);
		REG->CPSR ()->SetZero (result == 0);
		REG->CPSR ()->SetNegative ((result & 0x80000000) > 0);
	}
	
	// TODO: Calculate cycles.
}

////////////////////////////////////////////////////////////
// Single Data Transfer (LDR, STR)
////////////////////////////////////////////////////////////
void ARM60CPU::ProcessSingleDataTransfer (uint instruction)
{
	#ifdef __FOURDODEBUG__
	LastResult = "Single DT";
	#endif

	//////////////////////
	//  3         2         1         0
	// 10987654321098765432109876543210
	// Cond  I U W [Rn]    [  Offset  ]
	//     01 P B L    [Rd]
	//            (base)
	//                (dest)
	///////////////////////////

	RegisterType baseRegNum;
	
	bool  newCarry = false;
	int   offset;
	uint* baseReg;
	uint  address;

	////////////////////////////////////////////////////////
	baseRegNum = (RegisterType) ((instruction & 0x000F0000) >> 16);
	baseReg = REG->Reg (baseRegNum);

	// TODO: Special cases around use of R15 (prefetch).
	// TODO: Abort logic.

	////////////////////
	// Read offset.
	if ((instruction & 0x02000000) > 0)
	{
		// NOTE: The register-specified shift amounts are not available
		//       in this instruction class.
		offset = ReadShiftedRegisterOperand (instruction, &newCarry);
	}
	else
	{
		// Offset is an immediate value.
		offset = instruction & 0x00000FFF;
	}

	// Set offset positive/negative.
	if ((instruction & 0x00800000) == 0)
	{
		// We're supposed to subtract the offset.
		offset = -offset;
	}

	#ifdef __FOURDODEBUG__
	wxLogMessage (wxString::Format ("Offset is %i", offset));
	#endif

	if ((instruction & 0x01000000) > 0)
	{
		// Pre-indexed.
		// offset modification is performed before the base is used as the address.
		
		if ((instruction & 0x00200000) > 0)
		{
			// Write it back to the base register before we begin.
			*(baseReg) += offset;
			address = *(baseReg);
		}
		else
		{
			// Just use the base and offset. The base register is preserved.
			address = *(baseReg) + offset;
		}
	}
	else
	{
		// Post-indexed. 
		// offset modification is performed after the base is used as the address.
		// I don't see what this means... doesn't mean the offset just isn't done?
		address = *(baseReg);

		// NOTE: Use of the write-back bit is meaningless here except for 
		//       some mention of this as a practice in privileged mode, where
		//       setting the W bit forces non-privileged mode for the transfer,
		//       "allowing the operating system to generate a user address
		//       in a system where the memory management hardware makes suitable
		//       use of this hardware"
	}

	/////////////////////////////
	if ((instruction & 0x00100000) > 0)
	{
		///////////////////////
		// LDR Operation
		
		RegisterType regToLoad;
		
		// Account for prefetch!
		if( baseRegNum == RegisterType::ARM60_PC )
			address += 4;
		
		// Do LDR operation.
		regToLoad = (RegisterType) ( ( instruction & 0x0000F000 ) >> 12 );
		if( regToLoad == RegisterType::ARM60_PC )
		{
			// Load into PC, but only as a multiple of 4.
			*(REG->Reg( regToLoad )) = 
				DoLDR (address, (instruction & 0x00400000) > 0)
				& 0xfffffffc;
			
			// This is gonna cost a little extra...
			m_cycleCount += SCYCLES + NCYCLES;
		}
		else
		{
			// Typical load operation
			*(REG->Reg( regToLoad )) = DoLDR (address, (instruction & 0x00400000) > 0);
		}
		
		// Add a typical load's cycle usage.
		m_cycleCount += SCYCLES + NCYCLES + ICYCLES;
	}
	else
	{
		///////////////////////
		// STR Operation
		DoSTR (address, (RegisterType) ((instruction & 0x0000F000) >> 12), (instruction & 0x00400000) > 0);

		// Add STR cycle usage.
		m_cycleCount += ( NCYCLES * 2 );
	}
	
	if ((instruction & 0x00200000) > 0)
	{
		// Write it back to the base register after the transfer has completed.
		*(baseReg) += offset;
	}
}

////////////////////////////////////////////////////////////
// Block Data Transfer (LDM, STM)
////////////////////////////////////////////////////////////
void ARM60CPU::ProcessBlockDataTransfer (uint instruction)
{
	#ifdef __FOURDODEBUG__
	LastResult = "Block DT";
	#endif

	//////////////////////
	//  3         2         1         0
	// 10987654321098765432109876543210
	// Cond   P S L    [ Register List]
	//     100 U W [Rn]
	//
	// P = Pre/Post indexing   = 0x01000000
	// U = Up/Down (1=up)      = 0x00800000
	// S = PSR & force user    = 0x00400000
	// W = Write-back (1=yes)  = 0x00200000
	// L = Load/Store (1=load) = 0x00100000
	///////////////////////////

	bool  preIndex;
	bool  isWriteBack;
	bool  isPSR;
	bool  isLoad;
	bool  isR15used;
	int   increment;
	int   registerCount=0;
	int   reg;
	uint* baseReg;
	uint* destReg;
	uint  address;

	// Going up or down?
	increment = (instruction & 0x00800000) > 0 ? 4 : -4;
	
	// Find out how many registers we're storing.
	for (reg = 0; reg < 16; reg++)
	{
		if ((instruction & (int) pow((double) 2, reg)) > 0)
		{
			registerCount++;
		}
	}

	// See if R15 is being used.
	isR15used = (instruction & 0x00008000) > 0;  
	
	// Get some more parameters.
	preIndex = (instruction & 0x01000000) > 0; 
	isWriteBack = (instruction & 0x00200000) > 0;
	isPSR = (instruction & 0x00400000) > 0;
	isLoad = (instruction & 0x00100000) > 0;
	
	// Get base address.
	baseReg = REG->Reg ((RegisterType) (instruction & 0x000F0000));
	address = *baseReg;

	/////////////////////////
	// Start storing/loading registers.
	for (reg = 0; reg < 16; reg++)
	{
		if ((instruction & (int) pow((double) 2, reg)) > 0)
		{
			////////////////////
			// Perform preindexing?
			if (preIndex)
			{
				// We are preindexing. Update address first.
				address += increment;
				if (isWriteBack)
					*baseReg = address;
			}
			
			////////////////////
			// Determine destination/source.
			// The selected register is affected by the S bit and use of R15.
			if (isPSR)
			{
				if (isR15used)
				{
					if (isLoad)
					{
						// SPSR_mode is transferred to CPSR at the same as R15 is loaded.

						// In all other respectes, this mode is normal.
						destReg = REG->Reg ((RegisterType) reg);
					}
					else
					{
						// Use the user mode's registers regardless of current mode.
						destReg = REG->Reg ((InternalRegisterType) reg);
					}
				}
				else
				{
					// Use the user mode's registers regardless of current mode.
					destReg = REG->Reg ((InternalRegisterType) reg);
				}
			}
			else
			{
				// Normal operation.
				destReg = REG->Reg ((RegisterType) reg);
			}

			////////////////////
			// Perform operation here.
			if (isLoad)
			{
				*(destReg) = DMA->GetWord (address);
				if (isPSR && isR15used && isLoad)
				{
					// SPSR_mode is transferred to CPSR at the same as R15 is loaded.
					*(REG->Reg (ARM60_CPSR)) = *(REG->Reg (ARM60_SPSR));
				}
			}
			else
			{
				DMA->SetWord (address, *(destReg));
			}

			////////////////////
			// Perform postindexing?
			if (!preIndex)
			{
				// We are postindexing. Update address last.
				address += increment;
				if (isWriteBack)
					*baseReg = address;
			}
		}
	}
}

////////////////////////////////////////////////////////////
// Single data swap (SWP)
////////////////////////////////////////////////////////////
void ARM60CPU::ProcessSingleDataSwap (uint instruction)
{
	#ifdef __FOURDODEBUG__
	LastResult = "Data Swap";
	#endif
	
	///////////////////////////
	//  3         2         1         0
	// 10987654321098765432109876543210
	// Cond     B  [Rn]    00001001
	//     00010 00    [Rd]        [Rm]
	//       (Byte?)  (Dest)     (Source)
	//            (Base)
	//
	// B = Byte/Word (1=byte)  = 0x00400000
	///////////////////////////

	RegisterType baseReg;
	RegisterType destReg;
	RegisterType sourceReg;
	uint         value;
	uint         address;
	bool         isByte;

	baseReg = (RegisterType) (instruction & 0x000F0000 >> 16);
	sourceReg = (RegisterType) (instruction & 0x0000000F);
	destReg = (RegisterType) (instruction & 0x0000F000 >> 12);
	isByte = (instruction & 0x00400000) > 0;

	///////
	// This is supposed to be an atomic operation. 
	// The DMA should check this LOCK bit!

	LOCK = true;
	
	address = *(REG->Reg (baseReg));
	
	value = DoLDR (address, isByte);
	DoSTR (address, sourceReg, isByte);
	*(REG->Reg (destReg)) = value;

	LOCK = false;
	
	// Calculate cycle usage.
	m_cycleCount += SCYCLES + (2 * NCYCLES) + ICYCLES;
}

////////////////////////////////////////////////////////////
// Software Interrupt (SWI)
////////////////////////////////////////////////////////////
void ARM60CPU::ProcessSoftwareInterrupt (uint instruction)
{
	#ifdef __FOURDODEBUG__
	LastResult = "Software Int";
	#endif
	
	///////////////////////////
	//  3         2         1         0
	// 10987654321098765432109876543210
	// Cond    [Comment Field(ignored)]
	//     1111
	///////////////////////////
	
	//REG->

	//TODO: Software interrupt.
}

////////////////////////////////////////////////////////////
// Coproccesor data operations (CDP)
////////////////////////////////////////////////////////////
void ARM60CPU::ProcessCoprocessorDataOperations (uint instruction)
{
	#ifdef __FOURDODEBUG__
	LastResult = "Coproc DO";
	#endif
	
	///////////////////////////
	//  3         2         1         0
	// 10987654321098765432109876543210
	// Cond
	//     
	///////////////////////////
}

////////////////////////////////////////////////////////////
// Coproccesor data transfers (LDC, STC)
////////////////////////////////////////////////////////////
void ARM60CPU::ProcessCoprocessorDataTransfers (uint instruction)
{
	#ifdef __FOURDODEBUG__
	LastResult = "Coproc DT";
	#endif

	///////////////////////////
	//  3         2         1         0
	// 10987654321098765432109876543210
	// Cond
	//     
	///////////////////////////
}

////////////////////////////////////////////////////////////
// Coproccesor register transfers (MRC, MCR)
////////////////////////////////////////////////////////////
void ARM60CPU::ProcessCoprocessorRegisterTransfers (uint instruction)
{
	#ifdef __FOURDODEBUG__
	LastResult = "Coproc Reg";
	#endif

	///////////////////////////
	//  3         2         1         0
	// 10987654321098765432109876543210
	// Cond    CPO CRn_    CP#_   1
	//     1110   L    [Rd]    CP_ CRm_
	///////////////////////////
}

////////////////////////////////////////////////////////////
// Undefined instruction.
////////////////////////////////////////////////////////////
void ARM60CPU::ProcessUndefined (uint instruction)
{
	#ifdef __FOURDODEBUG__
	LastResult = "Undefined";
	#endif

	///////////////////////////
	//  3         2         1         0
	// 10987654321098765432109876543210
	// Cond   xxxxxxxxxxxxxxxxxxxx xxxx
	//     011                    1
	///////////////////////////
}

///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////

uint ARM60CPU::DoLDR (uint address, bool isByte)
{
	uint value;

	#ifdef __FOURDODEBUG__
	wxLogMessage (wxString::Format ("Read from address %i OR %u", address, address));
	#endif

	value = DMA->GetWord (address);
	
	if (isByte)
	{
		// We are loading a byte.

		// Loaded value depends on word boundaries.
		switch (BIGEND ? (3 - (address % 4)) : (address % 4))
		{
		case 0:
			value = (value & 0x000000FF);
			break;
		case 1:
			value = (value & 0x0000FF00) >> 8;
			break;
		case 2:
			value = (value & 0x00FF0000) >> 16;
			break;
		case 3:
			value = (value & 0xFF000000) >> 24;
			break;
		}
		
		// and all other bits are set to zero... done with shifting.
	}
	else
	{
		// Loading a word.
		if (BIGEND)
		{
			switch (address % 4)
			{
			case 0:
				// All good
				break;
			case 1:
				// Rotate value.
				value = (value >> 8) | (value << 24);
				break;
			case 2:
				// Rotate value.
				value = (value >> 16) | (value << 16);
				break;
			case 3:
				// Rotate value.
				value = (value >> 24) | (value << 8);
				break;
			}
		}
		else
		{
			switch (address % 4)
			{
			case 0:
				// All good
				break;
			case 1:
				// Rotate addressed byte to bits 15 through 8?
				break;
			case 2:
				// Rotate value.
				value = (value >> 16) | (value << 16);
				break;
			case 3:
				// Rotate addressed byte to bits 15 through 8?
				value = (value >> 8) | (value << 24);
				break;
			}
		}
	}
	return value;
}

void ARM60CPU::DoSTR (uint address, RegisterType sourceReg, bool isByte)
{
	// Store to memory!
	uint value;

	value = *(REG->Reg (sourceReg));
	
	// Account for prefetch.
	if (sourceReg == RegisterType::ARM60_PC)
		value += 8;
	
	if (isByte)
	{
		// Storing a byte...
		
		value = value & 0x000000FF;
		// NOTE: I used to think that we were supposed to store the
		//       bottom byte repeated four times. It didn't actually
		//       seem to happen this way.
		// value = value | (value << 8) | (value << 16) | (value << 24);
		// TODO: Track down the documentation that misled me here.
		// TODO: Is this supposed be be affected by big endian vs little?
		
		DMA->SetByte (address, (uchar) value);
	}
	else
	{
		// Storing a word. Easy.
		DMA->SetWord (address, value);
	}
	
	#ifdef __FOURDODEBUG__
	wxLogMessage (wxString::Format ("Storing the value [%u] into memory loc [%u]", value, address));
	#endif
}

uint ARM60CPU::ReadShiftedRegisterOperand (uint instruction, bool* newCarry)
{
	RegisterType regShift;
	RegisterType opReg;
	
	uint shiftType;
	uint op;
	int  shift;
	bool topBit;
	
	bool isRegShift;
	
	////////////////////////
	//  1 0 9 8 7 6 5 4 3 2 1 0
	// [     shift    ][  Rm  ]

	// Get the shift type.
	shiftType = (instruction & 0x00000060);

	// Get the value out of the register specified by Rm.
	opReg = (RegisterType) (instruction & 0x0000000F);
	op = *(REG->Reg (opReg));
	
	// Get the shift value's "type". (immediate vs. reg value)
	isRegShift = (instruction & 0x00000010) > 0;
	
	// Check for a prefetch case.
	if (opReg == RegisterType::ARM60_PC)
	{
		// We used PC, we need to add a prefetch value.
		if (isRegShift)
			op += 8;
		else
			op += 4;
	}
	
	// Check shift type to get the shift value.
	if (isRegShift)
	{
		// Shift by amount in bottom byte of a register

		regShift = (RegisterType) ((instruction & 0x00000F00) >> 8);
		shift = (*(REG->Reg (regShift))) & 0x000000FF;

		if (shift == 0)
		{
			// More documented special case processing. In this case, we force LSL 0!
			shiftType = 0;
		}
		
		// This takes an extra I cycle!
		m_cycleCount += ICYCLES;
	}
	else
	{
		// Shift by a certain amount.
		shift = (instruction & 0x00000F80) >> 7;
	}

	switch (instruction & 0x00000060)
	{
	case 0x00000000:
		// 00 = logical left
		if (shift == 0)
		{
			// This is a documented special case in which C is preserved.
			*newCarry = REG-> CPSR()->GetCarry ();
			op <<= shift;
		}
		else if (shift == 32)
		{
			*newCarry = (op & 0x00000001);
			op = 0;
		}
		else if (shift > 32)
		{
			*newCarry = false;
			op = 0;
		}
		else
		{
			// the carry bit will be one of the previous bits in this operand. 
			*newCarry = (op & (uint) pow ((double) 2, 31 - shift + 1)) > 0;
			op <<= shift;
		}
		break;

	case 0x00000020:
		// 01 = logical right
		if (shift == 0 || shift == 32)
		{
			// This is a documented special case in which the shift is actually 32.
			*newCarry = (op & 0x80000000) > 0;
			op = 0;
		}
		else if (shift > 32)
		{
			*newCarry = false;
			op = 0;
		}
		else
		{
			// Regular logical right shift.
			*newCarry = (op & (uint) pow ((double) 2, shift - 1)) > 0;
			op >>= shift;
		}
		break;

	case 0x00000040:
		// 10 = arithmetic right
		topBit = (op & 0x80000000) > 0;
		if (shift == 0 || shift >= 32)
		{
			// This is a documented special case in which the shift is actually 32.
			*newCarry = topBit;
			op = topBit ? 0xFFFFFFFF : 0;
		}
		else
		{
			// Regular logical right shift.
			*newCarry = (op & (uint) pow ((double) 2, shift - 1)) > 0;
			op = op >> shift;

			// Now set all the bits that were shifted in as the highest previous bit!
			if (topBit)
			{
				op |= ~((long) (pow ((double) 2, 32 - shift) - 1));
			}
		}
		break;

	case 0x00000060:
		// 11 = rotate right
		
		if (shift > 32)
		{
			// Modulus this down to size.
			shift %= 32;
			if (shift == 0)
			{
				shift = 32; 
			}
		}
		
		if (shift == 0)
		{
			// This is a special "Rotate Right Extended" in which everything
			// is shifted right one bit including the carry bit.
			*newCarry = (op & 0x00000001) > 0;
			op = (op >> 1) & (REG->CPSR ()->GetCarry () ? 0x80000000 : 0);
		}
		else
		{
			// Normal rotate;
			*newCarry = (op & (uint) pow ((double) 2, shift - 1)) > 0;
			op = (op >> shift) | (op << (32 - shift));
		}
		break;
	}

	return op;
}

uint ARM60CPU::DoAdd (uint op1, uint op2, bool oldCarry, bool* newCarry)
{
	uint lowerSum;
	uint higherSum;
	
	lowerSum = (op1 & 0x0000FFFF) + (op2 & 0x0000FFFF) + (oldCarry ? 1 : 0);
	higherSum =  ((op1 & 0xFFFF0000) >> 16) + ((op2 & 0xFFFF0000) >> 16) + (lowerSum >> 16);
	
	*newCarry = (higherSum & 0xFFFF0000) > 1;
	
	//TODO: Fix this hack
	if (((int) op1) == ((int) (-op2)))
	{
		return 0;
	}
	else
	{
		return op1 + op2 + (oldCarry ? 1 : 0);
	}
}