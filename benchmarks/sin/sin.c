#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

#include "mmio.h"
#include "font.h"
#include "PPM.h"

#include <math.h>

#define PPM_STATUS 0x4000
#define PPM_OPERATION 0x4004
#define PPM_ADDRESS 0x4008
#define PPM_DATA 0x400C
#define PPM_DUMP 0x4010
#define PPM_DOORBELL 0x4014

#define FRAME_WIDTH 320
#define FRAME_HEIGHT 240

#define CHAR_WIDTH 8
#define CHAR_HEIGHT 8

#define PI 3.14159265358979323846

// Frame buffer (RGB format, 32-bit per pixel)
unsigned int frame[FRAME_HEIGHT * FRAME_WIDTH];

int main(int argc, char *argv[])
{


  reg_write32(PPM_OPERATION, 1);
  for (int x = 0; x < FRAME_WIDTH; x++)
  {

    float scale_x = x * (2 * PI / FRAME_WIDTH);

    float y = sin(scale_x);

    int y_cord = (int)(y * 100) + FRAME_HEIGHT / 2;

    int pixel_address = y_cord * FRAME_WIDTH + x;

    reg_write32(PPM_DATA, 0xFFFFFF);
    reg_write32(PPM_ADDRESS, pixel_address);
    reg_write32(PPM_DOORBELL, 1);
  }

  printf("Done generating frame\n");

  reg_write32(PPM_DOORBELL, 1);
  reg_write32(PPM_DUMP, 1);

  return 0;
}
