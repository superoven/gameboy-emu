#include <stdio.h>
#include <stdlib.h>

#include "headers/gemu.h"

char* romdata;

int loadrom(const char* romname, char** result) {
  int size;
  FILE* fp;
  fp = fopen(romname, "rb");
  if (fp == NULL) error("File cannot be opened");
  
  fseek(fp, 0, SEEK_END);
  size = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  *result = (char *)malloc(size);

  if (size != fread(*result, sizeof(char), size, fp)) 
  { 
    free(*result);
    error("File buffered incorrectly");
  }
  
  fclose(fp);
  return size;
}

void printhex(int begin, int end) {
  for (int i = begin; i < end; i++) {
    printf("%04X:\t%01X%01X\n",i, (romdata[i]&0xF0)>>4, romdata[i]&0xF);
  }
}

const char* manufacturer(char data) {
  if (data == 0x33)
    return "Nintendo";
  if (data == 0x79)
    return "Accolade";
  if (data == 0xA4)
    return "Konami";
  return "Unknown";
}

const char* carttype(char data) {
  if (data == 0x0)
    return "ROM ONLY";
  if (data == 0x1)
    return "ROM+MBC1";
  if (data == 0x2)
    return "ROM+MBC1+RAM";
  if (data == 0x3)
    return "ROM+MBC1+RAM+BATTERY";
  if (data == 0x4)
    return "ROM+MBC2";
  if (data == 0x5)
    return "ROM+MBC2+BATTERY";
  return "Unknown";
}

int verifyrom() {
  return ((romdata[0x0100]&0xFF) == 0x00) && ((romdata[0x0101]&0xFF) == 0xC3);
}

//TODO: Checksum this properly
inline void rominfo(int size) {
  if (verifyrom()) {
    printf("Name of Game:\t%.15s\n", (romdata + 0x0134));
    printf("Size of File:\t%d bytes\n", size);
    printf("Language:\t%s\n", (romdata[0x014A]&0xFF) ? "English" : "Japanese");
    printf("Manufacturer:\t%s\n", manufacturer((romdata[0x014B]&0xFF)));
    printf("Version Number:\t1.%d\n", (romdata[0x014C]&0xFF));
  }
  else error("Invalid Rom");
}

/*
0100-0103  A sequence of bytes 00 C3 xx xx where last two bytes contain the
           starting address of a cartridge (lower byte first). The first two
           bytes of this sequence can be used as a "magic number" to
           recognize GameBoy cartridges. When GameBoy starts, the control is 
           passed to address 0100 and then the sequence is interpreted as    
           NOP; JP .
0104-0133  Nintendo character area:
           CE ED 66 66 CC 0D 00 0B 03 73 00 83 00 0C 00 0D
           00 08 11 1F 88 89 00 0E DC CC 6E E6 DD DD D9 99
           BB BB 67 63 6E 0E EC CC DD DC 99 9F BB B9 33 3E
0134-0143  Title of the game in ASCII terminated by zeroes
0144-0145  Extended manufacturer code in ASCII, when 33 stored at 014B
0146       Not used
0147       Cartridge type:
           0 - ROM ONLY         3 - ROM+MBC1+RAM+BATTERY
           1 - ROM+MBC1         5 - ROM+MBC2
           2 - ROM+MBC1+RAM     6 - ROM+MBC2+BATTERY
0148       ROM size:
           0 - 256kBit =  32kB =  2 banks
           1 - 512kBit =  64kB =  4 banks
           2 -   1MBit = 128kB =  8 banks
           3 -   2MBit = 256kB = 16 banks
           4 -   4MBit = 512kB = 32 banks
0149       RAM size:
           0 - None
           1 -  16kBit =  2kB = 1 bank
           2 -  64kBit =  8kB = 1 bank
           3 - 256kBit = 32kB = 4 banks
014A       Language:
           0 - Japanese game
           1 - English game
014B       Manufacturer code:
           33 - Nintendo or extended (see 0144-0145)
           79 - Accolade 
           A4 - Konami
014C       Version number
014D       Complement check
014E-014F  Checksum (higher byte first) produced by adding all bytes of   
           a cartridge except for two checksum bytes together and taking
           two lower bytes of the result.
*/
