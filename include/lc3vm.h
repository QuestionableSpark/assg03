/** @file lc3vm.h
 * @brief LC-3 VM API
 *
 * @author Nicholas Knebel
 * @note   cwid: 123456
 * @date   Spring 2026
 * @note   ide:  g++ 8.2.0 / GNU Make 4.2.1
 *
 * Header include file for LC-3 simulator API/functions.
 */
#include <stdbool.h>
#include <stdint.h>

#ifndef LC3VM_H
#define LC3VM_H

// total number of opcodes in the LC-3 architecture.
#define NUMOPS (16)

// Need to #define all of your bit manipulation macros like DR, SR1, etc. here.
#define FIMM(i) ((i >> 5) & 0x1)
#define FCND(i) (((i) >> 9) & 0x7)
#define BR(i) (((i) >> 6) & 0x7)
#define FL(i) (((i) >> 11) & 1)
#define TRP(i) ((i) & 0xFF)

typedef void (*op_ex_f)(uint16_t i);
typedef void (*trp_ex_f)();

enum
{
  trp_offset = 0x20
};

enum registr
{
  R0 = 0,
  R1,
  R2,
  R3,
  R4,
  R5,
  R6,
  R7,
  RPC,
  RCND,
  RCNT
};

enum flags
{
  FP = 1 << 0,
  FZ = 1 << 1,
  FN = 1 << 2
};

// If we are creating tests, make all declarations extern C so can
// work with catch2 C++ framework
#ifdef TEST
extern "C" {
#endif

extern bool running;
extern uint16_t mem[];
extern uint16_t reg[];
extern uint16_t PC_START;

// your task functions should go here
// Task 1: Implement Memory Access Functions
uint16_t mem_read(uint16_t address);
void mem_write(uint16_t address, uint16_t value);

// Task 2: Implement Sign Extension Function
uint16_t sign_extend(uint16_t bits, int size);

// Task 3: Declare Instruction/Operand Extraction Support Preprocessor Macros
#define OPC(i)      ((i) >> 12)
// Shifts the instruction right 12 bits to isolate the 4-bit opcode
// in the least significant bits, discarding all operand bits.

#define DR(i)       (((i) >> 9) & 0x7)
// Shifts the instruction right 9 bits then masks with 0x7 (0b111)
// to isolate the 3-bit destination register number in bits 11-9.

#define SR1(i)      (((i) >> 6) & 0x7)
// Shifts the instruction right 6 bits then masks with 0x7 (0b111)
// to isolate the 3-bit source register 1 number in bits 8-6.

#define SR2(i)      ((i) & 0x7)
// Masks the instruction with 0x7 (0b111) to isolate the 3-bit
// source register 2 number in the lowest bits 2-0.

#define SEXTIMM(i)  sign_extend((i) & 0x1F, 5)
// Masks the lowest 5 bits of the instruction and sign extends
// them to a full 16-bit twos-complement value. Used for immediate
// mode operands in add and andlc instructions.

#define OFF6(i)     sign_extend((i) & 0x3F, 6)
// Masks the lowest 6 bits of the instruction and sign extends
// them to a full 16-bit twos-complement value. Used as an offset
// from a base register in ldr and str instructions.

#define PCOFF9(i)   sign_extend((i) & 0x1FF, 9)
// Masks the lowest 9 bits of the instruction and sign extends
// them to a full 16-bits twos-complement value. Used as an offset
// from the PC in ld, st, ldi, sti, br and lea instructions.

#define PCOFF11(i)  sign_extend((i) & 0x7FF, 11)
// Masks the lowest 11 bits of the instruction and sign extends
// them to a full 16-bit twos-complement value. Used as an offset
// from the PC in the jsr instruction.

// Task 4: Declaring the update_flags() function
void update_flags(enum registr r);

// Task 5: Declaring the add(), andlc(), and notlc() functions
void add(uint16_t i);
void andlc(uint16_t i);
void notlc(uint16_t i);

// Task 6: Declaring various loading functions (load RPC + offset, 
// load indirect, load base + relative offset, and load effective address)
void ld(uint16_t i);
void ldi(uint16_t i);
void ldr(uint16_t i);
void lea(uint16_t i);

// Task 7: Declaring various storing functions (stores a value from a source
// register and stores it in memory)
void st(uint16_t i);
void sti(uint16_t i);
void str(uint16_t i);

void rti(uint16_t i);
void res(uint16_t i);
void tgetc();
void tout();
void tputs();
void tin();
void thalt();
void tinu16();
void toutu16();
void trap(uint16_t i);
void ld_img(char* fname, uint16_t offset);

#ifdef TEST
} // end extern C for C++ test runner
#endif

#endif // LC3VM_H
