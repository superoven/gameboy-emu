#include <inttypes.h>
#include <stdio.h>

#include "headers/gemu.h"
#include "headers/memory.h"

char* romdata;

int DISDEBUG = 1;

void debugoutput(unsigned char B1, unsigned char B2, unsigned char B3, unsigned char B4) {
  if (DISDEBUG) printf("ERROR - B1: %02X, B2: %02X, B3: %02X, B4: %02X\n", B1, B2, B3, B4);
} 

void printbuffer(uint16_t begin, uint16_t end, char* data) {
  while (begin < end) {
    printf("%04X: 0x%02X\n", begin, data[begin]);
    begin++;
  }
}

void disassemble(uint16_t begin, uint16_t end, char* data) {
  
  uint16_t jump = 0;
  while (begin < end) {
    jump = _disassemble(begin, data);
    if (!jump) {
      printf("(ERROR!)\n");
      break;
    }
    begin += jump;
  }
}

uint16_t _disassemble(uint16_t address, char* data)
{

  inst_t B1;
  B1.bits = *((uint8_t*)(data + address));


  /* One Byte Commands */
  /*********************/


  //**** LD r, (HL) ****
  if(B1.rtype.funct == 1 && B1.rtype.reg2 == 6)
  {
    printf("LD %s, (HL)\n", choosereg(B1.rtype.reg1));
    return 1;
  }
  
  //**** LD (HL), r ****
  if(B1.rtype.funct == 1 && B1.rtype.reg1 == 6)
  {
    printf("LD (HL), %s\n", choosereg(B1.rtype.reg2));
    return 1;
  }

  //**** LD r, r' ****
  if(B1.rtype.funct == 1)
  {
    printf("LD %s, %s\n", choosereg(B1.rtype.reg1), choosereg(B1.rtype.reg2));
    return 1;
  }

  //**** LD A, (BC) ****
  if(B1.bits == 0x0A)
  {
    printf("LD A, (BC)\n");
    return 1;
  }
  
  //**** LD A, (DE) ****
  if(B1.bits == 0x1A)
  {
    printf("LD A, (DE)\n");
    return 1;
  }

  //**** LD (BC), A ****
  if(B1.bits == 0x02)
  {
    printf("LD (BC), A\n");
    return 1;
  }

  //**** LD (DE), A ****
  if(B1.bits == 0x12)
  {
    printf("LD (DE), A\n");
    return 1;
  }
  
  //**** LD SP, HL ****
  if(B1.bits == 0xF9)
  {
    printf("LD SP, HL\n");
    return 1;
  }
  
  //**** EX DE, HL ****
  if(B1.bits == 0xEB)
  {
    printf("EX DE, HL\n");
    return 1;
  }
  
  //**** EX (SP), HL ****
  // EX H<->(SP+1),L<->(SP)
  if(B1.bits == 0xE3)
  {
    printf("EX (SP), HL\n");
    return 1;
  }

  //**** PUSH qq ****
  // PUSH (SP-2) <- qqL, (SP-1) <- qqH
  if(B1.ptype.funct == 3 && B1.ptype.footer == 5)
  {
    printf("PUSH %s\n", choosepair(B1.ptype.pair));
    return 1;
  }
  
  //**** POP qq ****
  // POP qqH <- (SP+1), qqL <- (SP)
  if(B1.ptype.funct == 3 && B1.ptype.footer == 1)
  {
    printf("POP %s\n", choosepair(B1.ptype.pair));
    return 1;
  }

  /* Two Byte Commands */
  /*********************/


  inst_t B2;
  B2.bits = *((uint8_t*)(data + address + 1));

  
  //**** LD (HL), n ****
  if(B1.bits == 0x36)
  {
    printf("LD (HL), %d\n", B2.bits);
    return 2;
  }

  //**** LD r, n ****
  if(B1.rtype.funct == 0 && B1.rtype.reg2 == 6)
  {
    printf("LD %s, %02X\n", choosereg(B1.rtype.reg1), B2.bits);
    return 2;
  }

  //**** LD A, I ****
  if(B1.bits == 0xED && B2.bits == 0x57)
  {
    printf("LD A, I\n");
    return 2;
  }
  
  //**** LD I, A ****
  if(B1.bits == 0xED && B2.bits == 0x47)
  {
    printf("LD I, A\n");
    return 2;
  }

  //**** LD A, R ****
  if(B1.bits == 0xED && B2.bits == 0x5F)
  {
    printf("LD A, R\n");
    return 2;
  }
  
  //**** LD R, A ****
  if(B1.bits == 0xED && B2.bits == 0x4F)
  {
    printf("LD R, A\n");
    return 2;
  }
  
  //**** LDI *****
  // LDI (DE)<-(HL),DE <- DE+1, HL <- HL+1, BC <- BC-1
  if(B1.bits == 0xED && B2.bits == 0xA0)
  {
    printf("LDI\n");
    return 2;
  }
  
  //**** LDD ****
  // LDD (DE)<-(HL),DE <- DE-1,HL <- HL-1,BC <- BC-1
  if(B1.bits == 0xED && B2.bits == 0xA8)
  {
    printf("LDD\n");
    return 2;
  }
  
  //**** LDDR ****
  // LDDR (DE)<-(HL),DE <- D <- 1,HL <- HL-1,BC <- BC-1
  if(B1.bits == 0xED && B2.bits == 0xB8)
  {
    printf("LDDR\n");
    return 2;
  }
  
  //**** CPDR ****
  // CPDR A-(HL),HL <- HL-1,BC <- BC-1
  if(B1.bits == 0xED && B2.bits == 0xB9)
  {
    printf("CPDR\n");
    return 2;
  }
  
  //**** LDIR ****
  // LDIR (DE)<-(HL),DE <- DE+1, HL <- HL+1, BC F <-> BC-1
  if(B1.bits == 0xED && B2.bits == 0xB0)
  {
    printf("LDIR\n");
    return 2;
  }
  
  //**** CPI ****
  // CPI A-(HL),HL <- HL+1, BC <- BC-1
  if(B1.bits == 0xED && B2.bits == 0xA1)
  {
    printf("CPI\n");
    return 2;
  }
  
  //**** CPIR ****
  // CPIR A-(HL),HL <- HL+1,BC <- BC-1
  if(B1.bits == 0xED && B2.bits == 0xB1)
  {
    printf("CPIR\n");
    return 2;
  }
  
  //**** CPD ****
  // CPD A-(HL),HL <- HL-1,BC <- BC-1
  if(B1.bits == 0xED && B2.bits == 0xA9)
  {
    printf("CPD\n");
    return 2;
  }

  /* Three Byte Commands */
  /***********************/


  inst_t B3;
  B3.bits = *((uint8_t*)(data + address + 2));


  //**** LD A, (nn) ****
  if(B1.bits == 0x3A)
  {
    printf("LD A, (%04X)\n", tobigend2(B2.bits, B3.bits));
    return 3;
  }

  //**** LD (nn), A ****
  if(B1.bits == 0x32)
  {
    printf("LD (%04X), A\n", tobigend2(B2.bits, B3.bits));
    return 3;
  }
  
  //**** LD HL, (nn) ****
  // H <- (nn+1) , L <- (nn)
  if(B1.bits == 0x2A)
  {
    printf("LD HL, (%04X)\n", tobigend2(B2.bits, B3.bits));
    return 3;
  }
  
  //**** LD (nn), HL ****
  //(nn+1) <- H, (nn) <- L
  if(B1.bits == 0x22)
  {
    printf("LD (%04X), HL\n", tobigend2(B2.bits, B3.bits));
    return 3;
  }
  
  //**** LD dd, nn ****
  if(B1.ptype.funct == 0 && B1.ptype.footer == 1)
  {
    printf("LD %s, %d\n", choosepair(B1.ptype.pair), tobigend2(B2.bits, B3.bits));
    return 3;
  }

  /* Four Byte Commands */
  /**********************/


  inst_t B4;
  B4.bits = *((uint8_t*)(data + address + 3));

  
  //**** LD dd, (nn) ****
  // ddh <- (nn+1) ddl <- (nn)
  if(B1.bits == 0xED && B2.ptype.funct == 1 && B2.ptype.footer == 0xB)
  {
    printf("LD %s, (%04X)\n", choosepair(B2.ptype.pair), tobigend2(B3.bits, B4.bits));
    return 4;
  }
  
  //**** LD (nn), dd ****
  // (nn+1) <- ddh, (nn) <- ddl
  if(B1.bits == 0xED && B2.ptype.funct == 1 && B2.ptype.footer == 3)
  {
    printf("LD (%04X), %s\n", tobigend2(B3.bits, B4.bits), choosepair(B2.ptype.pair));
    return 4;
  }

  debugoutput(B1.bits, B2.bits, B3.bits, B4.bits);
  return 0;
}
