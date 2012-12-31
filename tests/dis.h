#include "../src/headers/gemu.h"
#include "../src/headers/memory.h"
#include <string.h>
#include <stdlib.h>

void runtest(const char* input) {

  unsigned int numbytes = strlen(input)/2;

  // Make a buffer to be disassembled that contains input
  char* data = loadbytes(input);
  
  // Disassemble all the data
  disassemble(0, numbytes, data);
  
  free(data);
}
