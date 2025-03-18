#include "mmio.h"

#define PPM_STATUS 0x4000
#define PPM_OPERATION 0x4004
#define PPM_ADDRESS 0x4008
#define PPM_DATA 0x400C
#define PPM_DUMP 0x4010
#define PPM_DOORBELL 0x4014


int main(void)
{

  uint32_t result, ref, x = 20, y = 15;

  // wait for peripheral to be ready
  while (reg_read8(PPM_STATUS) == 0) ;

  reg_write32(PPM_DATA, 0xFF);
  reg_write32(PPM_OPERATION, 1);

  for(int i=0;i<320*20;i++){
    reg_write32(PPM_ADDRESS, i);
    reg_write32(PPM_DOORBELL, 1);
  }


  reg_write32(PPM_DUMP, 1);

  return 0;
}