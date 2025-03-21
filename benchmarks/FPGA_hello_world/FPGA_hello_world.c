#include <stdio.h>

#include "mmio.h"

#define ULTRASCALE_FPGA

int main(int argc, char *argv[])
{

  printf("Hello world!\n");


  while(1){
    *(volatile uint8_t *)0xFF000020 = 0x41;
  }



  return 0;
}
