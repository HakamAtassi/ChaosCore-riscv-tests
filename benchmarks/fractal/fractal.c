#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

#include "mmio.h"
#include "font.h"

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




// Frame buffer (RGB format, 32-bit per pixel)
unsigned int frame[FRAME_HEIGHT * FRAME_WIDTH];

// Set pixel in the frame buffer (32-bit RGB)
void set_pixel(int address, unsigned int RGB)
{
  frame[address] = RGB;
}

// Render a character to the frame at position (x, y)
void render_char(int x, int y, const unsigned char* bitmap)
{
    for (int row = 0; row < CHAR_HEIGHT; row++)
    {
        for (int col = 0; col < CHAR_WIDTH; col++)
        {
            if (bitmap[row] & (1 << col))
            {
                set_pixel((y + row) * FRAME_WIDTH + (x + col), 0xFFFFFF); 
            }
        }
    }
}

void dump_frame()   // moves frame buffer from core to device
{
    while (reg_read8(PPM_STATUS) == 0)
        ;

    reg_write32(PPM_OPERATION, 1);

    for (int i = 0; i < FRAME_WIDTH * 30; i++) 
    {
        reg_write32(PPM_DATA, frame[i]);
        reg_write32(PPM_ADDRESS, i);
        reg_write32(PPM_DOORBELL, 1);
    }

    reg_write32(PPM_DUMP, 1);
}

void update_frame() // requests PPM module to dump ppm
{
    reg_write32(PPM_DUMP, 1);
}



int main(int argc, char* argv[])
{

    printf("running fractcal\n");

    /* The window in the plane. */
    const float xmin = 0.27085;    // atof(argv[1]);
    const float xmax = 0.27100;    // atof(argv[2]);
    const float ymin = 0.004640;   // atof(argv[3]);
    const float ymax = 0.004810;   // atof(argv[4]);
    const uint16_t maxiter = 10; //(unsigned short)atoi(argv[5]);
    const int xres = 320;         // atoi(argv[6]);

    const int yres = (xres * (ymax - ymin)) / (xmax - xmin);


  /* Precompute pixel width and height. */
  float dx = (xmax - xmin) / xres;
  float dy = (ymax - ymin) / yres;

  /* Pixel and iteration variables */
  int i, j, k;
  float x, y, u, v, u2, v2;

  uint32_t address = 0;

  for (j = 0; j < yres; j++) {
    y = ymax - j * dy;
    for (i = 0; i < xres; i++) {
      x = xmin + i * dx;
      u = 0.0;
      v = 0.0;
      u2 = 0.0;
      v2 = 0.0;

      /* Mandelbrot iteration */
      for (k = 0; k < maxiter && (u2 + v2 < 4.0); k++) {
        v = 2 * u * v + y;
        u = u2 - v2 + x;
        u2 = u * u;
        v2 = v * v;
      }

      /* Compute pixel color */
      uint32_t color;
      if (k == maxiter) {
        /* Inside Mandelbrot set -> black */
        color=0;
      } else {
        /* Smooth coloring for outside points */
        float t = (float)k / maxiter;
        char red=(unsigned char)(9 * (1 - t) * t * t * t * 255);  // Red
        char green=(unsigned char)(15 * (1 - t) * (1 - t) * t * t * 255);  // Green
        char blue=(unsigned char)(8.5 * (1 - t) * (1 - t) * (1 - t) * t * 255);  // Blue

        color = (red<<16) | (green < 8) | (blue);

      }

      address+=1;

      printf("Setting pixel (%0d)\n", address);
      set_pixel(address, color);

    }
  }

  printf("Done generating frame\n");

  dump_frame();

  return 0;
}

