/*
  memory.c
  ----
  Handle all functions of the virtual
  memory of the gameboy.
  ----
*/

#include "headers/memory.h"
#include "headers/gemu.h"

#include <string.h>
#include <stdlib.h>

uint8_t regA = 0;
uint8_t regB = 0;
uint8_t regC = 0;
uint8_t regD = 0;
uint8_t regE = 0;
uint8_t regH = 0;
uint8_t regL = 0;

const char* choosereg(unsigned char x) {
  switch(x) {
  case 7:
    return "A";
    break;
  case 0:
    return "B";
    break;
  case 1:
    return "C";
    break;
  case 2:
    return "D";
    break;
  case 3:
    return "E";
    break;
  case 4:
    return "H";
    break;
  case 5:
    return "L";
    break;
  default:
    return "(UNKNOWN REGISTER)";
    break;
  }
}

const char* choosepair(unsigned char x) {
  switch(x) {
  case 0:
    return "BC";
    break;
  case 1:
    return "DE";
    break;
  case 2:
    return "HL";
    break;
  case 3:
    return "SP";
    break;
  default:
    return "(Unknown Pair)";
    break;
  }
}

char* loadbytes(const char* input) {
  if (strlen(input)%2) error("Incorrect input to function 'loadbytes'");
  char* buffer = malloc(strlen(input)/2);
  char *dst = buffer;
  char *end = buffer + sizeof(buffer);
  unsigned int u;
  while (dst < end && sscanf(input, "%2X", &u) == 1) {
    *dst++ = u;
    input += 2;
  }
  return buffer;
}
