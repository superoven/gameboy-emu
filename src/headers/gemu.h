/*
  gemu.h
  ----
  Header file for all things that 
  need to be linked to the main gameboy.
  ----
*/

#ifndef __gemu_h
#define __gemu_h

#include <stdio.h>

//The global pointer to the beginning of the rom data
extern char* romdata;

//General method to throw runtime errors
extern void error(const char* message);

//Load a rom file name "romname" into "result", return the size of the rom
extern int loadrom(const char* romname, char** result);

//Print out all basic information about this rom
extern void rominfo(int size);

//Print out the hex of the rom starting from "begin" to "end"
extern void printhex(int begin, int end);

#endif
