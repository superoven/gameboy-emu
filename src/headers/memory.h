/*
  memory.h
  ----
  Define the structs that provide mappings
  of all memory that the gameboy will need
  for the processor outside of the graphics
  card.
  ----
*/

#ifndef __memory_h
#define __memory_h

#include <inttypes.h>

extern uint8_t regA;
extern uint8_t regB;
extern uint8_t regC;
extern uint8_t regD;
extern uint8_t regE;
extern uint8_t regH;
extern uint8_t regL;

//Return the name of the register
const char* choosereg(unsigned char x);

#endif
