#include <fstream.h>
#include <iostream.h>
#include <math.h>

#include "types.h"
#include "BitMath.h"
#include "ARM60CPU.h"
#include "ARM60REGISTER.h"
#include "ARM60REGISTERS.h"

ARM60CPU::ARM60CPU ()
{
   m_reg = new ARM60Registers ();

   // NOTE TO GLORIOUS ALL-KNOWING JOHNNY (YOURSELF):
   //
   // All images scanned so far seem to start code at BYTE 72 (a.k.a. 73)
   //
   // "iamaduck" seems to be a NO-OP in many images? ... What the hell?
   //
   // TODO: Must double-check little-endian vs big-endian... "kcudamai"?

   /*
   /////////
   
   ifstream romFile;
   unsigned char*    buffer;
   int      length = 256;
   int x;

   //romFile.open ("C:\\emulation\\3do\\ROMS\\Trip'd (1995)(Panasonic)(Eu-US)[!].iso");
   //romFile.open ("C:\\emulation\\3do\\ROMS\\Out of this World (1993)(Interplay)(US)[!][45097-1].iso");
   //romFile.open ("C:\\emulation\\3do\\ROMS\\Alone in the Dark (1994)(Interplay)(US)[!].iso");
   //romFile.open ("C:\\emulation\\3do\\ROMS\\Lost Eden (1993)(Virgin)(US).iso");

   buffer = new unsigned char [length];
   romFile.read (buffer, length);

   for (x = 0; x < length; x++)
   {
      cout << x << "\tchar:\t" << (int) buffer [x] << "\t" << buffer [x] << "\t" << CharToBitString (buffer [x]) << endl;
   }

   cout << "---------------" << endl;

   for (x = 0; x < length; x+=4)
   {
      cout << x << "\t" << CharToBitString (buffer [x]) << CharToBitString (buffer [x + 1]) << CharToBitString (buffer [x + 2]) << CharToBitString (buffer [x + 3]) << "  " << CharToBitString (buffer [x + 3]) << CharToBitString (buffer [x + 2]) << CharToBitString (buffer [x + 1]) << CharToBitString (buffer [x]) << endl;
   }

   int y = -15;
   uint y2;

   unsigned char y3;
   
   y3= y;
   y2 = (uint) y3;

   cout << y << "\t" << y2 << endl;
   
   romFile.close ();
   delete buffer;
   */
}

ARM60CPU::~ARM60CPU ()
{
   delete m_reg;
}

void ARM60CPU::ProcessInstruction (uint instruction)
{
   if      ((instruction & 0x0C000000) == 0x00000000)
   {
      // TODO: Fix this! Ambiguous with multiple and single data swap!!

      // Data Processing PSR Transfer (00)
      this->ProcessDataProcessing (instruction);
   }
   else if ((instruction & 0x0C000000) == 0x04000000)
   {
      // Single Data Transfer (01)
   }
   else if ((instruction & 0x0E000000) == 0x06000000)
   {
      // Undefined (011)
   }
   else if ((instruction & 0x0E000000) == 0x08000000)
   {
      // Block Data Transfer (100)
   }
   else if ((instruction & 0x0E000000) == 0x0A000000)
   {
      // Branch (101)
      this->ProcessBranch (instruction);
   }
   else if ((instruction & 0x0E000000) == 0x0C000000)
   {
      // Coproc Data Transfer (110)
   }
   else if ((instruction & 0x0F000000) == 0x0F000000)
   {
      // Software Interrupt
   }
   else if ((instruction & 0x0F000010) == 0x0E000010)
   {
      // Coproc Register Transfer (1110 ... 1)
   }
   else if ((instruction & 0x0F000010) == 0x0E000000)
   {
      // Coproc Data Operation (1110 ... 0)
   }
   else if ((instruction & 0x0F800000) == 0x01000000)
   {
      // Single Data Swap
   }
   else if ((instruction & 0x0FC00000) == 0x00000000)
   {
      // Multiply
   }
   else
   {
      // Aw shit. Now what do we do?
   }
}

void ARM60CPU::ProcessBranch (uint instruction)
{
   // Check condition Field.
   if (! CheckCondition (instruction))
      return;

   ///////////////////////
   int  offset;

   if ((instruction & 0x01000000) == 0x01000000)
   {
      // TODO: Check on prefetch logic.
      *(m_reg->Reg (ARM60_R14)) = *(m_reg->PC ()->Value);
   }
   
   // Offset is bit shifted left two, then sign extended to 32 bits.
   offset = (instruction & 0x00FFFFFF) << 0;
   if ((offset & 0x02000000) > 0)
   {
      offset &= 0xFC000000;
   }
   (*(m_reg->PC ()->Value)) += offset;
}

void ARM60CPU::ProcessDataProcessing (uint instruction)
{
   //////////////////////
   // 10987654321098765432109876543210
   // Cond  I    S    [Rd]
   //     00 OpCd [Rn]    [ Operand 2]

   // Check condition Field.
   if (! CheckCondition (instruction))
      return;
   
   //////////////////////
   RegisterType regShift;

   int  opCode;
   bool setCond;
   uint op1;
   uint op2;
   uint shiftType;
   int  regDest;
   int  shift;
   bool newCarry;
   bool topBit;

   opCode = (instruction & 0x01E00000) >> 21;
   setCond = (instruction & 0x00100000) > 0;
   
   regDest = (instruction & 0x000F000) >> 12;

   ////////////////////////////
   // Get first operand value.
   op1 = *(m_reg->Reg ((RegisterType) (instruction & 0x000F0000)));
   
   ////////////////////////////
   // Get second operand value.

   // Check Immediate Operand.
   if ((instruction & 0x02000000) > 0)
   {
      // Immediate - operand 2 is an immediate value.
      //  1 0 9 8 7 6 5 4 3 2 1 0
      // [Rotate][ImmediateValue]
   }
   else
   {
      // Not immediate - Operand 2 is a shifted register.
      //  1 0 9 8 7 6 5 4 3 2 1 0
      // [     shift    ][  Rm  ]

      // Get the shift type.
      shiftType = (instruction & 0x00000060);

      // Check shift type to get the shift value.
      if ((instruction & 0x00000010) > 0)
      {
         // Shift by amount in bottom byte of a register
         regShift = (RegisterType) ((instruction & 0x00000F00) >> 8);
         shift = (*(m_reg->Reg (regShift))) & 0x000000FF;

         if (shift == 0)
         {
            // More documented special case processing. In this case, we force LSL 0!
            shiftType = 0;
         }
      }
      else
      {
         // Shift by a certain amount.
         shift = (instruction & 0x00000F80) >> 7;
      }
      
      // Get the value out of the register specified by Rm.
      op2 = *(m_reg->Reg ((RegisterType) (instruction & 0x0000000F)));

      switch (instruction & 0x00000060)
      {
      case 0x00000000:
         // 00 = logical left
         if (shift == 0)
         {
            // This is a documented special case in which C is preserved.
            newCarry = m_reg-> CPSR()->GetCarry ();
            op2 <<= shift;
         }
         else if (shift == 32)
         {
            newCarry = (op2 & 0x00000001);
            op2 = 0;
         }
         else if (shift > 32)
         {
            newCarry = false;
            op2 = 0;
         }
         else
         {
            // the carry bit will be one of the previous bits in this operand. 
            newCarry = (op2 & (uint) pow (2, 31 - shift + 1)) > 0;
            op2 <<= shift;
         }
         break;

      case 0x00000020:
         // 01 = logical right
         if (shift == 0 || shift == 32)
         {
            // This is a documented special case in which the shift is actually 32.
            newCarry = (op2 & 0x80000000) > 0;
            op2 = 0;
         }
         else if (shift > 32)
         {
            newCarry = false;
            op2 = 0;
         }
         else
         {
            // Regular logical right shift.
            newCarry = (op2 & (uint) pow (2, shift - 1)) > 0;
            op2 >>= shift;
         }
         break;

      case 0x00000040:
         // 10 = arithmetic right
         topBit = (op2 & 0x80000000) > 0;
         if (shift == 0 || shift >= 32)
         {
            // This is a documented special case in which the shift is actually 32.
            newCarry = topBit;
            op2 = topBit ? 0xFFFFFFFF : 0;
         }
         else
         {
            // Regular logical right shift.
            newCarry = (op2 & (uint) pow (2, shift - 1)) > 0;
            op2 = op2 >> shift;

            // Now set all the bits that were shifted in as the highest previous bit!
            if (topBit)
            {
               op2 |= ~((long) (pow (2, 32 - shift) - 1));
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
            newCarry = (op2 & 0x00000001) > 0;
            op2 = (op2 >> 1) & (m_reg-> CPSR()->GetCarry () ? 0x80000000 : 0);
         }
         else
         {
            // Normal rotate;
            newCarry = (op2 & (uint) pow (2, shift - 1)) > 0;
            op2 = (op2 >> shift) | (op2 << (32 - shift));
         }
         break;
      }
   }

   switch (opCode)
   {
   case 0x0:
      // AND
      return;
   
   case 0x1:
      // EOR
      return;
   
   case 0x2:
      // SUB
      return;
   
   case 0x3:
      // RSB
      return;
   
   case 0x4:
      // ADD
      return;
   
   case 0x5:
      // ADC
      return;
   
   case 0x6:
      // SBC
      return;
   
   case 0x7:
      // RSC
      return;
   
   case 0x8:
      // TST
      return;
   
   case 0x9:
      // TEQ
      return;
   
   case 0xA:
      // CMP
      return;
   
   case 0xB:
      // CMN
      return;
   
   case 0xC:
      // ORR
      return;
   
   case 0xD:
      // MOV
      return;

   case 0xE:
      // BIC
      return;

   case 0xF:
      // MVN
      return;

   }
}

bool ARM60CPU::CheckCondition (uint instruction)
{
   ARM60PSRegister* CPSR = m_reg->CPSR ();
   uint cond;

   cond = instruction & 0xF0000000;

   switch (cond)
   {
   case 0x00000000:
      // EQ - Z set (equal)
      return CPSR->GetZero ();
   
   case 0x10000000:
      // NE - Z clear (not equal)
      return !CPSR->GetZero ();
   
   case 0x20000000:
      // CS - C set (unsigned higher or same)
      return CPSR->GetCarry ();
   
   case 0x30000000:
      // CC - C clear (unsigned lower)
      return !CPSR->GetCarry ();
   
   case 0x40000000:
      // MI - N set (negative)
      return CPSR->GetNegative ();
   
   case 0x50000000:
      // PL - N clear (positive or zero)
      return !CPSR->GetNegative ();
   
   case 0x60000000:
      // VS - V set (overflow)
      return CPSR->GetOverflow ();
   
   case 0x70000000:
      // VC - V clear (no overflow)
      return !CPSR->GetOverflow ();
   
   case 0x80000000:
      // HI - C set and Z clear (unsigned higher)
      return CPSR->GetCarry () && (!CPSR->GetZero ());
   
   case 0x90000000:
      // LS - C clear or Z set (unsigned lower or same)
      return (!CPSR->GetCarry ()) || CPSR->GetZero ();
   
   case 0xA0000000:
      // GE - N set and V set, or N clear and V clear (greater or equal)
      return (CPSR->GetNegative ()&& CPSR->GetOverflow ())
            || ((!CPSR->GetNegative ()) && (!CPSR->GetOverflow ()));
   
   case 0xB0000000:
      // LT - N set and V clear, or N clear and V set (less than)
      return (CPSR->GetNegative  () && (!CPSR->GetOverflow ()))
            || ((!CPSR->GetNegative ()) && CPSR->GetOverflow ());
   
   case 0xC0000000:
      // GT - Z clear, and either N set and V set, or N clear and V clear (greater than)
      return (!CPSR->GetZero ())
            && ((CPSR->GetNegative  () && CPSR->GetOverflow ())
               || ((!CPSR->GetNegative ()) && (!CPSR->GetOverflow ())));
   
   case 0xD0000000:
      // LE - Z set, or N set and V clear, or N clear and V set (less than or equal)
      return CPSR->GetZero ()
            || (CPSR->GetNegative  () && (!CPSR->GetOverflow ()))
            || ((!CPSR->GetNegative ()) && CPSR->GetOverflow ());

   case 0xE0000000:
      // AL - always
      return true;

   case 0xF0000000:
      // NV - never
      
      // NOTE: The documentation specifies that the NV condition should
      //       not be used because it will be redifined in later ARM
      //       versions.
      return false;
   }

   // NOTE: The documentation specifies that the absence of a condition
   //       code acts as though "always" had been specified, although
   //       I don't see how this is possible.
   return true;
}