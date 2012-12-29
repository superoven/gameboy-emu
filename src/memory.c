/*
  memory.c
  ----
  Handle all functions of the virtual
  memory of the gameboy.
  ----
*/

#include "headers/memory.h"
#include "headers/gemu.h"

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
    return "(REGISTER)";
    break;
  }
}

