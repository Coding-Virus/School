/***************************************************************/
/*                                                             */
/*   ARMv4-32 Instruction Level Simulator                      */
/*                                                             */
/*   ECEN 4243                                                 */
/*   Oklahoma State University                                 */
/*                                                             */
/***************************************************************/

#ifndef _SIM_ISA_H_
#define _SIM_ISA_H_
#define N_CUR ( (CURRENT_STATE.CPSR>>31) & 0x00000001 )
#define Z_CUR ( (CURRENT_STATE.CPSR>>30) & 0x00000001 )
#define C_CUR ( (CURRENT_STATE.CPSR>>29) & 0x00000001 )
#define V_CUR ( (CURRENT_STATE.CPSR>>28) & 0x00000001 )
#define N_NXT ( (NEXT_STATE.CPSR>>31) & 0x00000001 )
#define Z_NXT ( (NEXT_STATE.CPSR>>30) & 0x00000001 )
#define C_NXT ( (NEXT_STATE.CPSR>>29) & 0x00000001 )
#define V_NXT ( (NEXT_STATE.CPSR>>28) & 0x00000001 )

#define N_N 0x80000000
#define Z_N 0x40000000
#define C_N 0x20000000
#define V_N 0x10000000

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"

int ADD (int Rd, int Rn, int Operand2, int I, int S, int CC)   //0100
{
  int cur = 0;
  if(I == 0) {
    int sh = (Operand2 & 0x00000060) >> 5;
    int shamt5 = (Operand2 & 0x00000F80) >> 7;
    int bit4 = (Operand2 & 0x00000010) >> 4;
    int Rm = Operand2 & 0x0000000F;
    int Rs = (Operand2 & 0x00000F00) >> 8;
    if (bit4 == 0) 
      switch (sh) {
      case 0: cur = CURRENT_STATE.REGS[Rn] + 
	  (CURRENT_STATE.REGS[Rm] << shamt5);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] + 
	  (CURRENT_STATE.REGS[Rm] >> shamt5);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] + 
	      ((CURRENT_STATE.REGS[Rm] << shamt5) |
               (CURRENT_STATE.REGS[Rm] >> (32 - shamt5)));
    	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] + 
	      ((CURRENT_STATE.REGS[Rm] >> shamt5) |
               (CURRENT_STATE.REGS[Rm] << (32 - shamt5)));
	  break;
      }     
    else
      switch (sh) {
      case 0: cur = CURRENT_STATE.REGS[Rn] + 
	  (CURRENT_STATE.REGS[Rm] << CURRENT_STATE.REGS[Rs]);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] + 
	  (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] + 
	      ((CURRENT_STATE.REGS[Rm] << CURRENT_STATE.REGS[Rs]) |
               (CURRENT_STATE.REGS[Rm] >> (32 - CURRENT_STATE.REGS[Rs])));
	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] + 
	      ((CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]) |
               (CURRENT_STATE.REGS[Rm] << (32 - CURRENT_STATE.REGS[Rs])));
	  break;
      }      
  }
  if (I == 1) {
    int rotate = Operand2 >> 8;
    int Imm = Operand2 & 0x000000FF;
    cur = CURRENT_STATE.REGS[Rn] + (Imm>>2*rotate|(Imm<<(32-2*rotate)));
  }
  NEXT_STATE.REGS[Rd] = cur;
  if (S == 1) {
    if (cur < 0)
      NEXT_STATE.CPSR |= N_N;
    if (cur == 0)
      NEXT_STATE.CPSR |= Z_N;
  }	
  return 0;

}

int ADC (int Rd, int Rn, int Operand2, int I, int S, int CC) // 0101
{
  int cur = 0;
  if(I == 0) {
    int sh = (Operand2 & 0x00000060) >> 5;
    int shamt5 = (Operand2 & 0x00000F80) >> 7;
    int bit4 = (Operand2 & 0x00000010) >> 4;
    int Rm = Operand2 & 0x0000000F;
    int Rs = (Operand2 & 0x00000F00) >> 8;
    int C = (Operand2 & 0x20000000) >> 29;
    if (bit4 == 0) 
      switch (sh) {
      case 0: cur = CURRENT_STATE.REGS[Rn] + 
	  (CURRENT_STATE.REGS[Rm] << shamt5) + C;
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] - 
	  (CURRENT_STATE.REGS[Rm] >> shamt5) + C;
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] +  
	      ((CURRENT_STATE.REGS[Rm] << shamt5) |
               (CURRENT_STATE.REGS[Rm] >> (32 - shamt5))) + C;
    	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] + 
	      ((CURRENT_STATE.REGS[Rm] >> shamt5) |
               (CURRENT_STATE.REGS[Rm] << (32 - shamt5))) + C;
	  break;
      }     
    else
      switch (sh) {
      case 0: cur = CURRENT_STATE.REGS[Rn] + 
	  (CURRENT_STATE.REGS[Rm] << CURRENT_STATE.REGS[Rs]) + C;
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] +  
	  (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]) + C;
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] + 
	      ((CURRENT_STATE.REGS[Rm] << CURRENT_STATE.REGS[Rs]) |
               (CURRENT_STATE.REGS[Rm] >> (32 - CURRENT_STATE.REGS[Rs]))) + C;
	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] + 
	      ((CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]) |
               (CURRENT_STATE.REGS[Rm] << (32 - CURRENT_STATE.REGS[Rs]))) + C;
	  break;
      }      
  }
  if (I == 1) {
    int rotate = Operand2 >> 8;
    int Imm = Operand2 & 0x000000FF;
    cur = CURRENT_STATE.REGS[Rn] + (Imm>>2*rotate|(Imm<<(32-2*rotate)));
  }
  NEXT_STATE.REGS[Rd] = cur;
  if (S == 1) {
    if (cur < 0)
      NEXT_STATE.CPSR |= N_N;
    if (cur == 0)
      NEXT_STATE.CPSR |= Z_N;
  }	
  return 0;
}
int AND (int Rd, int Rn, int Operand2, int I, int S, int CC) // 0000
{
  int cur = 0;
  if(I == 0) {
    int sh = (Operand2 & 0x00000060) >> 5;
    int shamt5 = (Operand2 & 0x00000F80) >> 7;
    int bit4 = (Operand2 & 0x00000010) >> 4;
    int Rm = Operand2 & 0x0000000F;
    int Rs = (Operand2 & 0x00000F00) >> 8;
    if (bit4 == 0) 
      switch (sh) {
      case 0: cur = CURRENT_STATE.REGS[Rn] & 
	  (CURRENT_STATE.REGS[Rm] << shamt5);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] & 
	  (CURRENT_STATE.REGS[Rm] >> shamt5);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] & 
	      ((CURRENT_STATE.REGS[Rm] << shamt5) |
               (CURRENT_STATE.REGS[Rm] >> (32 - shamt5)));
    	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] & 
	      ((CURRENT_STATE.REGS[Rm] >> shamt5) |
               (CURRENT_STATE.REGS[Rm] << (32 - shamt5)));
	  break;
      }     
    else
      switch (sh) {
      case 0: cur = CURRENT_STATE.REGS[Rn] & 
	  (CURRENT_STATE.REGS[Rm] << CURRENT_STATE.REGS[Rs]);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] & 
	  (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] & 
	      ((CURRENT_STATE.REGS[Rm] << CURRENT_STATE.REGS[Rs]) |
               (CURRENT_STATE.REGS[Rm] >> (32 - CURRENT_STATE.REGS[Rs])));
	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] & 
	      ((CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]) |
               (CURRENT_STATE.REGS[Rm] << (32 - CURRENT_STATE.REGS[Rs])));
	  break;
      }      
  }
  if (I == 1) {
    int rotate = Operand2 >> 8;
    int Imm = Operand2 & 0x000000FF;
    cur = CURRENT_STATE.REGS[Rn] + (Imm>>2*rotate|(Imm<<(32-2*rotate)));
  }
  NEXT_STATE.REGS[Rd] = cur;
  if (S == 1) {
    if (cur < 0)
      NEXT_STATE.CPSR |= N_N;
    if (cur == 0)
      NEXT_STATE.CPSR |= Z_N;
  }	
  return 0;
}
int RSB (int Rd, int Rn, int Operand2, int I, int S, int CC)   // 0011
{
  int cur = 0;
  if(I == 0) {
    int sh = (Operand2 & 0x00000060) >> 5;
    int shamt5 = (Operand2 & 0x00000F80) >> 7;
    int bit4 = (Operand2 & 0x00000010) >> 4;
    int Rm = Operand2 & 0x0000000F;
    int Rs = (Operand2 & 0x00000F00) >> 8;
    if (bit4 == 0) 
      switch (sh) {
      case 0: cur = (CURRENT_STATE.REGS[Rm] << shamt5) - 
      CURRENT_STATE.REGS[Rn];
	  break;
      case 1: (CURRENT_STATE.REGS[Rm] >> shamt5) - 
      cur = CURRENT_STATE.REGS[Rn];
	  
	  break;
      case 2: cur = ((CURRENT_STATE.REGS[Rm] << shamt5) |
               (CURRENT_STATE.REGS[Rm] >> (32 - shamt5))) - 
               CURRENT_STATE.REGS[Rn];
    	  break;
      case 3: cur = ((CURRENT_STATE.REGS[Rm] >> shamt5) |
               (CURRENT_STATE.REGS[Rm] << (32 - shamt5))) - 
               CURRENT_STATE.REGS[Rn];
	  break;
      }     
    else
      switch (sh) {
      case 0: cur = (CURRENT_STATE.REGS[Rm] << CURRENT_STATE.REGS[Rs]) - 
      CURRENT_STATE.REGS[Rn];
	  break;
      case 1: cur = (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]) - 
      CURRENT_STATE.REGS[Rn];
	  break;
      case 2: cur = ((CURRENT_STATE.REGS[Rm] << CURRENT_STATE.REGS[Rs]) |
                (CURRENT_STATE.REGS[Rm] >> (32 - CURRENT_STATE.REGS[Rs]))) - 
                CURRENT_STATE.REGS[Rn];
	  break;
      case 3: ((CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]) |
               (CURRENT_STATE.REGS[Rm] << (32 - CURRENT_STATE.REGS[Rs]))) - 
               cur = CURRENT_STATE.REGS[Rn];
	  break;
      }      
  }
  if (I == 1) {
    int rotate = Operand2 >> 8;
    int Imm = Operand2 & 0x000000FF;
    cur = CURRENT_STATE.REGS[Rn] + (Imm>>2*rotate|(Imm<<(32-2*rotate)));
  }
  NEXT_STATE.REGS[Rd] = cur;
  if (S == 1) {
    if (cur < 0)
      NEXT_STATE.CPSR |= N_N;
    if (cur == 0)
      NEXT_STATE.CPSR |= Z_N;
  }	
  return 0;
}
int ASR (int Rd, int Rn, int Operand2, int I, int S, int CC)   // 1101, I = 0, sh = 10
{
    int sh = (Operand2 & 0x00000060) >> 5;
    int shamt5 = (Operand2 & 0x00000F80) >> 7;
    int bit4 = (Operand2 & 0x00000010) >> 4;
    int Rm = Operand2 & 0x0000000F;
    int Rs = (Operand2 & 0x00000F00) >> 8;
}
int B (int Rd, int Rn, int Operand2, int I, int S, int CC)  // L = 0
{
    int sh = (Operand2 & 0x00000060) >> 5;
    int shamt5 = (Operand2 & 0x00000F80) >> 7;
    int bit4 = (Operand2 & 0x00000010) >> 4;
    int Rm = Operand2 & 0x0000000F;
    int Rs = (Operand2 & 0x00000F00) >> 8;
}
int BIC (int Rd, int Rn, int Operand2, int I, int S, int CC)   // 1110
{
  int cur = 0;
  if(I == 0) {
    int sh = (Operand2 & 0x00000060) >> 5;
    int shamt5 = (Operand2 & 0x00000F80) >> 7;
    int bit4 = (Operand2 & 0x00000010) >> 4;
    int Rm = Operand2 & 0x0000000F;
    int Rs = (Operand2 & 0x00000F00) >> 8;
    int sh = (Operand2 & 0x00000060) >> 5;
    int shamt5 = (Operand2 & 0x00000F80) >> 7;
    int bit4 = (Operand2 & 0x00000010) >> 4;
    int Rm = Operand2 & 0x0000000F;
    int Rs = (Operand2 & 0x00000F00) >> 8;
    if (bit4 == 0) 
      switch (sh) {
      case 0: cur = CURRENT_STATE.REGS[Rn] & 
	  ~(CURRENT_STATE.REGS[Rm] << shamt5);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] & 
	  ~(CURRENT_STATE.REGS[Rm] >> shamt5);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] & 
	      ~((CURRENT_STATE.REGS[Rm] << shamt5) |
               (CURRENT_STATE.REGS[Rm] >> (32 - shamt5)));
    	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] & 
	      ~((CURRENT_STATE.REGS[Rm] >> shamt5) |
               (CURRENT_STATE.REGS[Rm] << (32 - shamt5)));
	  break;
      }     
    else
      switch (sh) {
      case 0: cur = CURRENT_STATE.REGS[Rn] & 
	  (CURRENT_STATE.REGS[Rm] << CURRENT_STATE.REGS[Rs]);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] & 
	  ~(CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] & 
	      ~((CURRENT_STATE.REGS[Rm] << CURRENT_STATE.REGS[Rs]) |
               (CURRENT_STATE.REGS[Rm] >> (32 - CURRENT_STATE.REGS[Rs])));
	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] & 
	      ~((CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]) |
               (CURRENT_STATE.REGS[Rm] << (32 - CURRENT_STATE.REGS[Rs])));
	  break;
      }      
  }
  if (I == 1) {
    int rotate = Operand2 >> 8;
    int Imm = Operand2 & 0x000000FF;
    cur = CURRENT_STATE.REGS[Rn] + (Imm>>2*rotate|(Imm<<(32-2*rotate)));
  }
  NEXT_STATE.REGS[Rd] = cur;
  if (S == 1) {
    if (cur < 0)
      NEXT_STATE.CPSR |= N_N;
    if (cur == 0)
      NEXT_STATE.CPSR |= Z_N;
  }	
  return 0;
}
int BL (int Rd, int Rn, int Operand2, int I, int S, int CC)   // L = 1
{
    int sh = (Operand2 & 0x00000060) >> 5;
    int shamt5 = (Operand2 & 0x00000F80) >> 7;
    int bit4 = (Operand2 & 0x00000010) >> 4;
    int Rm = Operand2 & 0x0000000F;
    int Rs = (Operand2 & 0x00000F00) >> 8;
}
int CMN (int Rd, int Rn, int Operand2, int I, int S, int CC)  // 1011, S = 1
{
    int sh = (Operand2 & 0x00000060) >> 5;
    int shamt5 = (Operand2 & 0x00000F80) >> 7;
    int bit4 = (Operand2 & 0x00000010) >> 4;
    int Rm = Operand2 & 0x0000000F;
    int Rs = (Operand2 & 0x00000F00) >> 8;
}
int CMP (int Rd, int Rn, int Operand2, int I, int S, int CC)   // 1010, S = 1
{   
    int sh = (Operand2 & 0x00000060) >> 5;
    int shamt5 = (Operand2 & 0x00000F80) >> 7;
    int bit4 = (Operand2 & 0x00000010) >> 4;
    int Rm = Operand2 & 0x0000000F;
    int Rs = (Operand2 & 0x00000F00) >> 8;
}
int EOR (int Rd, int Rn, int Operand2, int I, int S, int CC)   // 0001
{
  int cur = 0;
  if(I == 0) {
    int sh = (Operand2 & 0x00000060) >> 5;
    int shamt5 = (Operand2 & 0x00000F80) >> 7;
    int bit4 = (Operand2 & 0x00000010) >> 4;
    int Rm = Operand2 & 0x0000000F;
    int Rs = (Operand2 & 0x00000F00) >> 8;
    if (bit4 == 0) 
      switch (sh) {
      case 0: cur = CURRENT_STATE.REGS[Rn] ^ 
	  (CURRENT_STATE.REGS[Rm] << shamt5);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] ^ 
	  (CURRENT_STATE.REGS[Rm] >> shamt5);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] ^ 
	      ((CURRENT_STATE.REGS[Rm] << shamt5) |
               (CURRENT_STATE.REGS[Rm] >> (32 - shamt5)));
    	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] ^ 
	      ((CURRENT_STATE.REGS[Rm] >> shamt5) |
               (CURRENT_STATE.REGS[Rm] << (32 - shamt5)));
	  break;
      }     
    else
      switch (sh) {
      case 0: cur = CURRENT_STATE.REGS[Rn] ^ 
	  (CURRENT_STATE.REGS[Rm] << CURRENT_STATE.REGS[Rs]);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] ^ 
	  (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] ^ 
	      ((CURRENT_STATE.REGS[Rm] << CURRENT_STATE.REGS[Rs]) |
               (CURRENT_STATE.REGS[Rm] >> (32 - CURRENT_STATE.REGS[Rs])));
	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] ^ 
	      ((CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]) |
               (CURRENT_STATE.REGS[Rm] << (32 - CURRENT_STATE.REGS[Rs])));
	  break;
      }      
  }
  if (I == 1) {
    int rotate = Operand2 >> 8;
    int Imm = Operand2 & 0x000000FF;
    cur = CURRENT_STATE.REGS[Rn] + (Imm>>2*rotate|(Imm<<(32-2*rotate)));
  }
  NEXT_STATE.REGS[Rd] = cur;
  if (S == 1) {
    if (cur < 0)
      NEXT_STATE.CPSR |= N_N;
    if (cur == 0)
      NEXT_STATE.CPSR |= Z_N;
  }	
  return 0;
}
int LDR (int Rd, int Rn, int Operand2, int I, int S, int CC)   // op = 01, B = 0, L = 1
{
    int sh = (Operand2 & 0x00000060) >> 5;
    int shamt5 = (Operand2 & 0x00000F80) >> 7;
    int bit4 = (Operand2 & 0x00000010) >> 4;
    int Rm = Operand2 & 0x0000000F;
    int Rs = (Operand2 & 0x00000F00) >> 8;
}
int LDRB (int Rd, int Rn, int Operand2, int I, int S, int CC)   // op = 01, B = 1, L = 1
{
    int sh = (Operand2 & 0x00000060) >> 5;
    int shamt5 = (Operand2 & 0x00000F80) >> 7;
    int bit4 = (Operand2 & 0x00000010) >> 4;
    int Rm = Operand2 & 0x0000000F;
    int Rs = (Operand2 & 0x00000F00) >> 8;
}
int LSL (int Rd, int Rn, int Operand2, int I, int S, int CC)   // 1101, I = 0, sh = 00
{
    int sh = (Operand2 & 0x00000060) >> 5;
    int shamt5 = (Operand2 & 0x00000F80) >> 7;
    int bit4 = (Operand2 & 0x00000010) >> 4;
    int Rm = Operand2 & 0x0000000F;
    int Rs = (Operand2 & 0x00000F00) >> 8;
}
int LSR (int Rd, int Rn, int Operand2, int I, int S, int CC)   // 1101, I = 0, sh = 01
{
    int sh = (Operand2 & 0x00000060) >> 5;
    int shamt5 = (Operand2 & 0x00000F80) >> 7;
    int bit4 = (Operand2 & 0x00000010) >> 4;
    int Rm = Operand2 & 0x0000000F;
    int Rs = (Operand2 & 0x00000F00) >> 8;
}
int MLA (int Rd, int Rn, int Operand2, int I, int S, int CC)   // 001
{
    int sh = (Operand2 & 0x00000060) >> 5;
    int shamt5 = (Operand2 & 0x00000F80) >> 7;
    int bit4 = (Operand2 & 0x00000010) >> 4;
    int Rm = Operand2 & 0x0000000F;
    int Rs = (Operand2 & 0x00000F00) >> 8;
}
int MOV (int Rd, int Rn, int Operand2, int I, int S, int CC)   // 1101, I = 1 OR instr(11:4) = 0
{
    int sh = (Operand2 & 0x00000060) >> 5;
    int shamt5 = (Operand2 & 0x00000F80) >> 7;
    int bit4 = (Operand2 & 0x00000010) >> 4;
    int Rm = Operand2 & 0x0000000F;
    int Rs = (Operand2 & 0x00000F00) >> 8;
}
int MUL (int Rd, int Rn, int Operand2, int I, int S, int CC)   // 000
{
    int sh = (Operand2 & 0x00000060) >> 5;
    int shamt5 = (Operand2 & 0x00000F80) >> 7;
    int bit4 = (Operand2 & 0x00000010) >> 4;
    int Rm = Operand2 & 0x0000000F;
    int Rs = (Operand2 & 0x00000F00) >> 8;
}
int MVN (int Rd, int Rn, int Operand2, int I, int S, int CC)   // 1111
{
  int cur = 0;
  if(I == 0) {
    int sh = (Operand2 & 0x00000060) >> 5;
    int shamt5 = (Operand2 & 0x00000F80) >> 7;
    int bit4 = (Operand2 & 0x00000010) >> 4;
    int Rm = Operand2 & 0x0000000F;
    int Rs = (Operand2 & 0x00000F00) >> 8;
    int cur = 0;
    if (bit4 == 0) 
      switch (sh) {
      case 0: cur = ~CURRENT_STATE.REGS[Rn];
	  break;
      case 1: cur = ~CURRENT_STATE.REGS[Rn];
	  break;
      case 2: cur = ~CURRENT_STATE.REGS[Rn];
    	  break;
      case 3: cur = ~CURRENT_STATE.REGS[Rn];
	  break;
      }     
    else
      switch (sh) {
      case 0: cur = ~CURRENT_STATE.REGS[Rn];
	  break;
      case 1: cur = ~CURRENT_STATE.REGS[Rn];
	  break;
      case 2: cur = ~CURRENT_STATE.REGS[Rn];
	  break;
      case 3: cur = ~CURRENT_STATE.REGS[Rn];
	  break;
      }      
  }
  if (I == 1) {
    int rotate = Operand2 >> 8;
    int Imm = Operand2 & 0x000000FF;
    cur = CURRENT_STATE.REGS[Rn] + (Imm>>2*rotate|(Imm<<(32-2*rotate)));
  }
  NEXT_STATE.REGS[Rd] = cur;
  if (S == 1) {
    if (cur < 0)
      NEXT_STATE.CPSR |= N_N;
    if (cur == 0)
      NEXT_STATE.CPSR |= Z_N;
  }	
  return 0;

}
int ORR (int Rd, int Rn, int Operand2, int I, int S, int CC)   //1100
{
  int cur = 0;
  if(I == 0) {
    int sh = (Operand2 & 0x00000060) >> 5;
    int shamt5 = (Operand2 & 0x00000F80) >> 7;
    int bit4 = (Operand2 & 0x00000010) >> 4;
    int Rm = Operand2 & 0x0000000F;
    int Rs = (Operand2 & 0x00000F00) >> 8;
    if (bit4 == 0) 
      switch (sh) {
      case 0: cur = CURRENT_STATE.REGS[Rn] | 
	  (CURRENT_STATE.REGS[Rm] << shamt5);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] | 
	  (CURRENT_STATE.REGS[Rm] >> shamt5);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] | 
	      ((CURRENT_STATE.REGS[Rm] << shamt5) |
               (CURRENT_STATE.REGS[Rm] >> (32 - shamt5)));
    	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] | 
	      ((CURRENT_STATE.REGS[Rm] >> shamt5) |
               (CURRENT_STATE.REGS[Rm] << (32 - shamt5)));
	  break;
      }     
    else
      switch (sh) {
      case 0: cur = CURRENT_STATE.REGS[Rn] | 
	  (CURRENT_STATE.REGS[Rm] << CURRENT_STATE.REGS[Rs]);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] | 
	  (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] | 
	      ((CURRENT_STATE.REGS[Rm] << CURRENT_STATE.REGS[Rs]) |
               (CURRENT_STATE.REGS[Rm] >> (32 - CURRENT_STATE.REGS[Rs])));
	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] | 
	      ((CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]) |
               (CURRENT_STATE.REGS[Rm] << (32 - CURRENT_STATE.REGS[Rs])));
	  break;
      }      
  }
  if (I == 1) {
    int rotate = Operand2 >> 8;
    int Imm = Operand2 & 0x000000FF;
    cur = CURRENT_STATE.REGS[Rn] + (Imm>>2*rotate|(Imm<<(32-2*rotate)));
  }
  NEXT_STATE.REGS[Rd] = cur;
  if (S == 1) {
    if (cur < 0)
      NEXT_STATE.CPSR |= N_N;
    if (cur == 0)
      NEXT_STATE.CPSR |= Z_N;
  }	
  return 0;
}
int ROR (int Rd, int Rn, int Operand2, int I, int S, int CC)   // 1101, I = 0, sh = 11
{
    int sh = (Operand2 & 0x00000060) >> 5;
    int shamt5 = (Operand2 & 0x00000F80) >> 7;
    int bit4 = (Operand2 & 0x00000010) >> 4;
    int Rm = Operand2 & 0x0000000F;
    int Rs = (Operand2 & 0x00000F00) >> 8;
}
int SBC (int Rd, int Rn, int Operand2, int I, int S, int CC)   // 0110
{
  int cur = 0;
  if(I == 0) {
    int sh = (Operand2 & 0x00000060) >> 5;
    int shamt5 = (Operand2 & 0x00000F80) >> 7;
    int bit4 = (Operand2 & 0x00000010) >> 4;
    int Rm = Operand2 & 0x0000000F;
    int Rs = (Operand2 & 0x00000F00) >> 8;
    int C = (Operand2 & 0x20000000) >> 29;
    if (bit4 == 0) 
      switch (sh) {
      case 0: cur = CURRENT_STATE.REGS[Rn] -
	  (CURRENT_STATE.REGS[Rm] << shamt5) - ~C;
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] - 
	  (CURRENT_STATE.REGS[Rm] >> shamt5) - ~C;
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] - 
	      ((CURRENT_STATE.REGS[Rm] << shamt5) |
               (CURRENT_STATE.REGS[Rm] >> (32 - shamt5))) - ~C;
    	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] - 
	      ((CURRENT_STATE.REGS[Rm] >> shamt5) |
               (CURRENT_STATE.REGS[Rm] << (32 - shamt5))) - ~C;
	  break;
      }     
    else
      switch (sh) {
      case 0: cur = CURRENT_STATE.REGS[Rn] - 
	  (CURRENT_STATE.REGS[Rm] << CURRENT_STATE.REGS[Rs]) - ~C;
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] - 
	  (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]) - ~C;
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] - 
	      ((CURRENT_STATE.REGS[Rm] << CURRENT_STATE.REGS[Rs]) |
               (CURRENT_STATE.REGS[Rm] >> (32 - CURRENT_STATE.REGS[Rs]))) - ~C;
	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] - 
	      ((CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]) |
               (CURRENT_STATE.REGS[Rm] << (32 - CURRENT_STATE.REGS[Rs]))) - ~C;
	  break;
      }      
  }
  if (I == 1) {
    int rotate = Operand2 >> 8;
    int Imm = Operand2 & 0x000000FF;
    cur = CURRENT_STATE.REGS[Rn] + (Imm>>2*rotate|(Imm<<(32-2*rotate)));
  }
  NEXT_STATE.REGS[Rd] = cur;
  if (S == 1) {
    if (cur < 0)
      NEXT_STATE.CPSR |= N_N;
    if (cur == 0)
      NEXT_STATE.CPSR |= Z_N;
  }	
  return 0;
}
int SBC (int Rd, int Rn, int Operand2, int I, int S, int CC)   // 0110
{
  int cur = 0;
  if(I == 0) {
    int sh = (Operand2 & 0x00000060) >> 5;
    int shamt5 = (Operand2 & 0x00000F80) >> 7;
    int bit4 = (Operand2 & 0x00000010) >> 4;
    int Rm = Operand2 & 0x0000000F;
    int Rs = (Operand2 & 0x00000F00) >> 8;
    int C = (Operand2 & 0x20000000) >> 29;
     if (bit4 == 0) 
      switch (sh) {
      case 0: cur = (CURRENT_STATE.REGS[Rm] << shamt5) - 
      CURRENT_STATE.REGS[Rn] - ~C;
	  break;
      case 1: (CURRENT_STATE.REGS[Rm] >> shamt5) - 
      cur = CURRENT_STATE.REGS[Rn] - ~C;
	  
	  break;
      case 2: cur = ((CURRENT_STATE.REGS[Rm] << shamt5) |
               (CURRENT_STATE.REGS[Rm] >> (32 - shamt5))) - 
               CURRENT_STATE.REGS[Rn] - ~C;
    	  break;
      case 3: cur = ((CURRENT_STATE.REGS[Rm] >> shamt5) |
               (CURRENT_STATE.REGS[Rm] << (32 - shamt5))) - 
               CURRENT_STATE.REGS[Rn] - ~C;
	  break;
      }     
    else
      switch (sh) {
      case 0: cur = (CURRENT_STATE.REGS[Rm] << CURRENT_STATE.REGS[Rs]) - 
      CURRENT_STATE.REGS[Rn] - ~C;
	  break;
      case 1: cur = (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]) - 
      CURRENT_STATE.REGS[Rn] - ~C;
	  break;
      case 2: cur = ((CURRENT_STATE.REGS[Rm] << CURRENT_STATE.REGS[Rs]) |
                (CURRENT_STATE.REGS[Rm] >> (32 - CURRENT_STATE.REGS[Rs]))) - 
                CURRENT_STATE.REGS[Rn] - ~C;
	  break;
      case 3: ((CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]) |
               (CURRENT_STATE.REGS[Rm] << (32 - CURRENT_STATE.REGS[Rs]))) - 
               cur = CURRENT_STATE.REGS[Rn] - ~C;
	  break;
      }      
  }
  if (I == 1) {
    int rotate = Operand2 >> 8;
    int Imm = Operand2 & 0x000000FF;
    cur = CURRENT_STATE.REGS[Rn] + (Imm>>2*rotate|(Imm<<(32-2*rotate)));
  }
  NEXT_STATE.REGS[Rd] = cur;
  if (S == 1) {
    if (cur < 0)
      NEXT_STATE.CPSR |= N_N;
    if (cur == 0)
      NEXT_STATE.CPSR |= Z_N;
  }	
  return 0;

}
int STR (int Rd, int Rn, int Operand2, int I, int S, int CC)   // op = 01, B = 0, L = 0
{
    int sh = (Operand2 & 0x00000060) >> 5;
    int shamt5 = (Operand2 & 0x00000F80) >> 7;
    int bit4 = (Operand2 & 0x00000010) >> 4;
    int Rm = Operand2 & 0x0000000F;
    int Rs = (Operand2 & 0x00000F00) >> 8;
}
int STRB (int Rd, int Rn, int Operand2, int I, int S, int CC)   // op = 01, B = 1, L = 0
{
    int sh = (Operand2 & 0x00000060) >> 5;
    int shamt5 = (Operand2 & 0x00000F80) >> 7;
    int bit4 = (Operand2 & 0x00000010) >> 4;
    int Rm = Operand2 & 0x0000000F;
    int Rs = (Operand2 & 0x00000F00) >> 8;
}
int SUB (int Rd, int Rn, int Operand2, int I, int S, int CC)   // 0010
{
  int cur = 0;
  if(I == 0) {
    int sh = (Operand2 & 0x00000060) >> 5;
    int shamt5 = (Operand2 & 0x00000F80) >> 7;
    int bit4 = (Operand2 & 0x00000010) >> 4;
    int Rm = Operand2 & 0x0000000F;
    int Rs = (Operand2 & 0x00000F00) >> 8;
    if (bit4 == 0) 
      switch (sh) {
      case 0: cur = CURRENT_STATE.REGS[Rn] - 
	  (CURRENT_STATE.REGS[Rm] << shamt5);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] - 
	  (CURRENT_STATE.REGS[Rm] >> shamt5);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] - 
	      ((CURRENT_STATE.REGS[Rm] << shamt5) |
               (CURRENT_STATE.REGS[Rm] >> (32 - shamt5)));
    	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] - 
	      ((CURRENT_STATE.REGS[Rm] >> shamt5) |
               (CURRENT_STATE.REGS[Rm] << (32 - shamt5)));
	  break;
      }     
    else
      switch (sh) {
      case 0: cur = CURRENT_STATE.REGS[Rn] - 
	  (CURRENT_STATE.REGS[Rm] << CURRENT_STATE.REGS[Rs]);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] - 
	  (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] - 
	      ((CURRENT_STATE.REGS[Rm] << CURRENT_STATE.REGS[Rs]) |
               (CURRENT_STATE.REGS[Rm] >> (32 - CURRENT_STATE.REGS[Rs])));
	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] - 
	      ((CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]) |
               (CURRENT_STATE.REGS[Rm] << (32 - CURRENT_STATE.REGS[Rs])));
	  break;
      }      
  }
  if (I == 1) {
    int rotate = Operand2 >> 8;
    int Imm = Operand2 & 0x000000FF;
    cur = CURRENT_STATE.REGS[Rn] + (Imm>>2*rotate|(Imm<<(32-2*rotate)));
  }
  NEXT_STATE.REGS[Rd] = cur;
  if (S == 1) {
    if (cur < 0)
      NEXT_STATE.CPSR |= N_N;
    if (cur == 0)
      NEXT_STATE.CPSR |= Z_N;
  }	
  return 0;
}
int TEQ (int Rd, int Rn, int Operand2, int I, int S, int CC)   // 1001, S = 1
{
    int sh = (Operand2 & 0x00000060) >> 5;
    int shamt5 = (Operand2 & 0x00000F80) >> 7;
    int bit4 = (Operand2 & 0x00000010) >> 4;
    int Rm = Operand2 & 0x0000000F;
    int Rs = (Operand2 & 0x00000F00) >> 8;
}
int TST (int Rd, int Rn, int Operand2, int I, int S, int CC)   // 1000, S = 1
{
    int sh = (Operand2 & 0x00000060) >> 5;
    int shamt5 = (Operand2 & 0x00000F80) >> 7;
    int bit4 = (Operand2 & 0x00000010) >> 4;
    int Rm = Operand2 & 0x0000000F;
    int Rs = (Operand2 & 0x00000F00) >> 8;
}
int SWI (int Rd, int Rn, int Operand2, int I, int S, int CC)
{
    int sh = (Operand2 & 0x00000060) >> 5;
    int shamt5 = (Operand2 & 0x00000F80) >> 7;
    int bit4 = (Operand2 & 0x00000010) >> 4;
    int Rm = Operand2 & 0x0000000F;
    int Rs = (Operand2 & 0x00000F00) >> 8;
    return 0;
}

#endif
