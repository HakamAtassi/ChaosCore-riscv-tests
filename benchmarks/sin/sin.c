#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

#include "mmio.h"
#include "font.h"
#include "PPM.h"

#include <math.h>

#define PI 3.14159265358979323846

// Frame buffer (RGB format, 32-bit per pixel)
unsigned int frame[FRAME_HEIGHT * FRAME_WIDTH];

int main(int argc, char *argv[])
{

  write_filename("rocketchip_sinwave.ppm");

  reg_write32(PPM_OPERATION, 1);
  for (int x = 0; x < FRAME_WIDTH; x++)
  {

    float scale_x = x * (2 * PI / FRAME_WIDTH);

    float y = sin(scale_x);

    int y_cord = (int)(y * FRAME_HEIGHT/2) + FRAME_HEIGHT / 2;

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
