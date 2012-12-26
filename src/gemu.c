/*
  gemu.c
  ----
  The emulator itself.
  Try to keep this file as simple as possible
  and move most if not all abstractions into other
  files.
  ----
*/

#include "headers/memory.h"
#include "headers/processor.h"
#include "headers/gemu.h"

#include <stdio.h>
#include <stdlib.h>

char* romdata;

void error(const char* message) {
  fprintf(stderr, "Error: %s\n", message);
  exit(1);
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Usage: ./gemu [rom]\n");
    return 0;
  }

  int filesize = loadrom(argv[1], &romdata);
  rominfo(filesize);
  return 0;
}
