#include <stdio.h>

#include "mmio.h"

#define ULTRASCALE_FPGA

#define UART1_ADDR 0xFF000020

void putch(char c){
  *(volatile uint8_t *)UART1_ADDR = c;
}

void UART1_printf(char* str){

  for(int i=0;str[i]!='\0';i++){
    putch(str[i]);
  }

}

int main(int argc, char *argv[])
{

  UART1_printf("Hello, world!");


  return 0;
}
