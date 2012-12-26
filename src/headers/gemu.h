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

extern void error(const char* message);
extern int loadrom(const char* romname, char** result);

extern char* romdata;
  
//Do stuff

#endif
