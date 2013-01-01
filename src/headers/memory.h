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

/* Byte commands of the form:
   00|000|000 (REGISTER type)
   With the second two used to
   choose between first a second
   register in a command */
typedef struct
{
  unsigned int reg2 : 3;
  unsigned int reg1 : 3;
  unsigned int funct : 2;
} rtype_inst_t;

/* Byte commands of the form:
   00|00|0000 (PUSH type)
   The second tells what pair
   the last portion is an 
   identifying footer */
typedef struct
{
  unsigned int footer : 4;
  unsigned int pair : 2;
  unsigned int funct : 2;
} ptype_inst_t;

/* The General instruction type:
   All instructions will load into
   this, and can be immediately
   diverted to any of the other
   struct forms that one would 
   like to interpret it as */
typedef union
{
  rtype_inst_t rtype;
  ptype_inst_t ptype;
  uint8_t bits;
} inst_t;

//Return the name of the register
const char* choosereg(unsigned char x);

//Return the pair of registers
const char* choosepair(unsigned char x);

//Take a string of hex values, and return a char* array of that data
extern char* loadbytes(const char* input);

//Convert to big endian
extern uint16_t tobigend2(uint8_t a, uint8_t b);
extern uint32_t tobigend3(uint8_t a, uint8_t b, uint8_t c);
extern uint32_t tobigend4(uint8_t a, uint8_t b, uint8_t c, uint8_t d);

#endif
