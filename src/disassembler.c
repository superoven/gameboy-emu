#include <inttypes.h>
#include <stdio.h>

#include "headers/gemu.h"
#include "headers/memory.h"

char* romdata;

int disassemble(uint16_t address)
{
/* Ok, so I'm just making variables for any bits that are used to differentiate opcodes.
    Once I've gone through all of the opcodes I will be able to organize it better
    Reads in the address to parse instruction at, returns number of bytes parsed
    variable names go B #of byte being read _ bit range being stored
*/
  unsigned char B1_07 = romdata[address] & 0xFF;    // byte 1 bits[0:7]
  unsigned char B1_01 = B1_07 >> 6;                 // byte 1 bits[0:1]
  unsigned char B1_24 =(B1_07 >> 3) & 7;            // byte 1 bits[2:4]
  unsigned char B1_47 = B1_07 & 0xF;                // byte 1 bits[4:7]
  unsigned char B1_57 = B1_07 & 7;                  // byte 1 bits[5:7]
  
  unsigned char B2_07 = romdata[address+1] & 0xFF;  // byte 2 bits[0:7]
  unsigned char B2_01 = B2_07 >> 6;                 // byte 2 bits[0:1]
  unsigned char B2_24 =(B2_07 >> 3) & 7;            // byte 2 bits[2:4]
  unsigned char B2_47 = B2_07 & 0xF;                // byte 2 bits[4:7]
  unsigned char B2_57 = B2_07 & 7;                  // byte 2 bits[5:7]
  
  unsigned char B3_07 = romdata[address+2] & 0xFF;  // byte 3 bits[0:7]
  unsigned char B3_01 = B3_07 >> 6;                 // byte 3 bits[0:1]
  unsigned char B3_24 =(B3_07 >> 3) & 7;            // byte 3 bits[2:4]
  unsigned char B3_47 = B3_07 & 0xF;                // byte 3 bits[4:7]
  unsigned char B3_57 = B3_07 & 7;                  // byte 3 bits[5:7]
  
  unsigned char B4_07 = romdata[address+3] & 0xFF;  // byte 4 bits[0:7]
  unsigned char B4_01 = B4_07 >> 6;                 // byte 4 bits[0:1]
  unsigned char B4_24 =(B4_07 >> 3) & 7;            // byte 4 bits[2:4]
  unsigned char B4_47 = B1_07 & 0xF;                // byte 4 bits[4:7]
  unsigned char B4_57 = B4_07 & 7;                  // byte 4 bits[5:7]
  
  //**** LD r, (HL) ****
  if(B1_01==1 && B1_57==6)
  {
    printf("LD %s <- (HL)\n", choosereg(B1_24));
    return 1;
  }
  
  //**** LD (HL), r ****
  if(B1_01==1 && B1_24==6)
  {
    printf("LD (HL) <- %s\n", choosereg(B1_57));
    return 1;
  }
  
  //**** LD r, r' ****
  if(B1_01 == 1)
  {
    printf("LD %s <- %s\n", choosereg(B1_24), choosereg(B1_57));
    return 1;
  }
  
  //**** LD (HL), n ****
  if(B1_07==0x36)
  {
    printf("LD (HL) <- 0x%08X\n", B2_07);
    return 2;
  }
  
  //**** LD A, (BC) ****
  if(B1_07==0x0A)
  {
    printf("LD A <- (BC)\n");
    return 1;
  }
  
  //**** LD A, (DE) ****
  if(B1_07==0x1A)
  {
    printf("LD A <- (DE)\n");
    return 1;
  }
  
  //**** LD A, (nn) ****
  if(B1_07==0x3A)
  {
    uint16_t result = (B3_07 << 8) + (B2_07);
    printf("LD A <- (%016X)\n", result);
    return 3;
  }

  //**** LD (nn), A ****
  if(B1_07==0x32)
  {
    uint16_t result = (B3_07 << 8) + (B2_07);
    printf("LD (%016X) <- A\n", result);
    return 3;
  }
  
  //**** LD (BC), A ****
  if(B1_07==0x02)
  {
    printf("LD (BC) <- A\n");
    return 1;
  }
  
  //**** LD (DE), A ****
  if(B1_07==0x12)
  {
    printf("LD (DE) <- A\n");
    return 1;
  }
  
  if(B1_07==0x2A)
  {
    printf("H <- (nn+1) , L <- (nn)\n");
    return 3;
  }
  
  if(B1_07==0x22)
  {
    printf("(nn+1) <- H, (nn) <- L\n");
    return 3;
  }
  
  if(B1_07==0xF9)
  {
    printf("LD SP <- HL\n");
    return 1;
  }
  
  if(B1_07==0xEB)
  {
    printf("EX DE <-> HL\n");
    return 1;
  }
  
  if(B1_07==0x08)
  {
    printf("EX AF <-> AF'\n");
    return 1;
  }
  
  if(B1_07==0xD9)
  {
    printf("EXX (BC)<->(BC'),(DE)<->(DE'),(HL)<->(HL')\n");
    return 1;
  }
  
  if(B1_07==0xE3)
  {
    printf("EX H<->(SP+1),L<->(SP)\n");
    return 1;
  }
  
  //**** LD dd nn ****
  if(B1_01==0 && B1_47==1)
  {
    printf("LD dd <- nn\n");
    return 3;
  }
  
  if(B1_01==3 && B1_47==5)
  {
    printf("PUSH (SP-2) <- qqL, (SP-1) <- qqH\n");
    return 1;
  }
  
  if(B1_01==3 && B1_47==1)
  {
    printf("POP qqH <- (SP+1), qqL <- (SP)\n");
    return 1;
  }
  
  if(B1_01==0 && B1_57==6)
  {
    printf("LD r <- n\n");
    return 2;
  }
  
  if(B1_07==0xED && B2_07==0x57)
  {
    printf("LD A <- I\n");
    return 2;
  }
  
  if(B1_07==0xED && B2_07==0x47)
  {
    printf("LD I <- A\n");
    return 2;
  }
  
  if(B1_07==0xED && B2_07==0x5F)
  {
    printf("LD A <- R\n");
    return 2;
  }
  
  if(B1_07==0xED && B2_07==0x4F)
  {
    printf("LD R <- A\n");
    return 2;
  }
  
  if(B1_07==0xDD && B2_07==0x36)
  {
    printf("LD (IX+d) <- n\n");
    return 4;
  }
  
  if(B1_07==0xDD && B2_07==0x21)
  {
    printf("LD IX <- nn\n");
    return 4;
  }
  
  if(B1_07==0xDD && B2_07==0x2A)
  {
    printf("IXh <- (nn+1), IXI <- (nn)\n");
    return 4;
  }
  
  if(B1_07==0xDD && B2_07==0x22)
  {
    printf("(nn+1) <- IXh, (nn) <- IXI\n");
    return 4;
  }
  
  if((B1_07==0xDD) && (B2_07==0xF9))
  {
    printf("LD SP <- -IX\n");
    return 2;
  }
  
  if(B1_07==0xDD && B2_07==0xE5)
  {
    printf("PUSH (SP-2) <- IXL, (SP-1) <- IXH\n");
    return 2;
  }
  
  if(B1_07==0xDD && B2_07==0xE1)
  {
    printf("POP IXH <- (SP+1), IXL <- (SP)\n");
    return 2;
  }
  
  if(B1_07==0xDD && B2_07==0xE3)
  {
    printf("EX IXH<->(SP+1),IXL<->(SP)\n");
    return 1;
  }
  
  if(B1_07==0xDD && B2_07==0xFD)
  {
    printf("POP IYH <- (SP-X1), IYL <- (SP)\n");
    return 2;
  }
  
  if(B1_07==0xED && B2_07==0xA0)
  {
    printf("LDI (DE)<-(HL),DE <- DE+1, HL <- HL+1, BC <- BC-1\n");
    return 2;
  }
  
  if(B1_07==0xED && B2_07==0xA8)
  {
    printf("LDD (DE)<-(HL),DE <- DE-1,HL <- HL-1,BC <- BC-1\n");
    return 2;
  }
  
  if(B1_07==0xED && B2_07==0xB8)
  {
    printf("LDDR (DE)<-(HL),DE <- D <- 1,HL <- HL-1,BC <- BC-1\n");
    return 2;
  }
  
  if(B1_07==0xED && B2_07==0xB9)
  {
    printf("CPDR A-(HL),HL <- HL-1,BC <- BC-1\n");
    return 2;
  }
  
  if(B1_07==0xED && B2_07==0xB0)
  {
    printf("LDIR (DE)<-(HL),DE <- DE+1, HL <- HL+1, BC F <-> BC-1\n");
    return 2;
  }
  
  if(B1_07==0xED && B2_07==0xA1)
  {
    printf("CPI A-(HL),HL <- HL+1, BC <- BC-1\n");
    return 2;
  }
  
  if(B1_07==0xED && B2_07==0xB1)
  {
    printf("CPIR A-(HL),HL <- HL+1,BC <- BC-1\n");
    return 2;
  }
  
  if(B1_07==0xED && B2_07==0xA9)
  {
    printf("CPD A-(HL),HL <- HL-1,BC <- BC-1\n");
    return 2;
  }
  
  if(B1_07==0xFD && B2_07==0x36)
  {
    printf("LD (IY+d) <- n\n");
    return 4;
  }
  
  if(B1_07==0xFD && B2_07==0x21)
  {
    printf("LD IY <- nn\n");
    return 4;
  }
  
  if(B1_07==0xFD && B2_07==0x22)
  {
    printf("(nn+1) <- IYh, (nn) <- IYI\n");
    return 4;
  }
  
  if(B1_07==0xFD && B2_07==0x2A)
  {
    printf("IYh <- (nn+1), IYI <- (nn)\n");
    return 4;
  }
  
  if(B1_07==0xFD && B2_07==0xE3)
  {
    printf("EX IYH<->(SP+1),IYL<->(SP)\n");
    return 2;
  }
  
  if(B1_07==0xFD && B2_07==0xE5)
  {
    printf("PUSH (SP-2) <- IYL, (SP-1) <- IYH\n");
    return 2;
  }
  
  if(B1_07==0xFD && B2_07==0xF9)
  {
    printf("LD SP <-IY\n");
    return 2;
  }
  
  if(B1_07==0xED && B2_01==1 && B2_47==0xB)
  {
    printf("ddh <- (nn+1) ddl <- (nn)\n");
    return 4;
  }
  
  if(B1_07==0xED && B2_01==1 && B2_47==3)
  {
    printf("(nn+1) <- ddh, (nn) <- ddl\n");
    return 4;
  }
  
  if(B1_07==0xDD && B2_01==1 && B2_24==6)
  {
    printf("LD (IX+d) <- r\n");
    return 3;
  }
  
  if(B1_07==0xDD && B2_01==1 && B2_57==6)
  {
    printf("LD r <- (IX+d)\n");
    return 3;
  }
  
  if(B1_07==0xFD && B2_01==1 && B2_24==6)
  {
    printf("LD (IY+d) <- r\n");
    return 3;
  }
  
  if(B1_07==0xFD && B2_01==1 && B2_57==6)
  {
    printf("LD r <- (IY+d)\n");
    return 3;
  }
  
  return 0;
}
