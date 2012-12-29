
int dissamle(uint16_t address)
{
/* Ok, so I'm just making variables for any bits that are used to differentiate opcodes.
    Once I've gone through all of the opcodes I will be able to organize it better
    Reads in the address to parse instruction at, returns number of bytes parsed
    variable names go B #of byte being read _ bit range being stored
*/
  unsigned char B1_07 = romdata[address];              // byte 1 bits[0:7]
  unsigned char B1_01 = romdata[address] >> 6;         // byte 1 bits[0:1]
  unsigned char B1_24 = (romdata[address] >> 3) & 7;   // byte 1 bits[2:4]
  unsigned char B1_57 = romdata[address] & 7;          // byte 1 bits[5:7]
  unsigned char B2_07 = romdata[address+1];            // byte 2 bits[0:7]
  unsigned char B2_01 = romdata[address+1] >> 6;       // byte 2 bits[0:1]
  unsigned char B2_24 = (romdata[address+1] >> 3) & 7; // byte 2 bits[2:4]
  unsigned char B2_57 = romdata[address+1] & 7;        // byte 2 bits[5:7]
  unsigned char B3_07 = romdata[address+2];            // byte 3 bits[0:7]
  unsigned char B3_01 = romdata[address+2] >> 6;       // byte 3 bits[0:1]
  unsigned char B3_24 = (romdata[address+2] >> 3) & 7; // byte 3 bits[2:4]
  unsigned char B3_57 = romdata[address+2] & 7;        // byte 3 bits[5:7]
  
  if(B1_01==1 && B1_57==6)
  {
    printf("LD r <- (HL)\n");
    return 1;
  }
  
  if(B1_01 == 1)
  {
    printf("LD r <- r'\n");
    return 1;
  }
  
  if(B1_01==0 && B1_57==6)
  {
    printf("LD r <- n\n");
    return 2;
  }
}
