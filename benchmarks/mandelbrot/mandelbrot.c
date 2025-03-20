#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

#include "PPM.h"

#include "mmio.h"
#include "font.h"

#define PPM_STATUS 0x4000
#define PPM_OPERATION 0x4004
#define PPM_ADDRESS 0x4008
#define PPM_DATA 0x400C
#define PPM_DUMP 0x4010
#define PPM_DOORBELL 0x4014


#define CHAR_WIDTH 8
#define CHAR_HEIGHT 8

// Frame buffer (RGB format, 32-bit per pixel)
unsigned int frame[FRAME_HEIGHT * FRAME_WIDTH];

int main(int argc, char *argv[])
{
  printf("running fractcal\n");

  write_filename("rocketchip_mandelbrot.ppm");

  /* The window in the plane. */
  const float xmin = -2; 
  const float xmax = 1.0; 
  const float ymin = -1.5;
  const float ymax = 1.5; 
  const uint16_t maxiter = 5;
  const int xres = 100;
  const int yres = 100;

  /* Precompute pixel width and height. */
  float dx = (xmax - xmin) / xres;
  float dy = (ymax - ymin) / yres;

  /* Pixel and iteration variables */
  int i, j, k;
  float x, y, u, v, u2, v2;

  for (j = 0; j < yres; j++)
  {
    y = ymax - j * dy;
    for (i = 0; i < xres; i++)
    {
      x = xmin + i * dx;
      u = 0.0;
      v = 0.0;
      u2 = 0.0;
      v2 = 0.0;

      /* Mandelbrot iteration */
      for (k = 0; k < maxiter && (u2 + v2 < 4.0); k++)
      {
        v = 2 * u * v + y;
        u = u2 - v2 + x;
        u2 = u * u;
        v2 = v * v;
      }

      /* Compute pixel color */
      uint32_t color;
      if (k == maxiter)
      {
        /* Inside Mandelbrot set -> black */
        color = 0;
      }
      else
      {
        /* Smooth coloring for outside points */
        float t = (float)k / maxiter;
        char red = (unsigned char)(9 * (1 - t) * t * t * t * 255);                // Red
        char green = (unsigned char)(15 * (1 - t) * (1 - t) * t * t * 255);       // Green
        char blue = (unsigned char)(8.5 * (1 - t) * (1 - t) * (1 - t) * t * 255); // Blue

        color = (red << 16) | (green << 8) | (blue);

        write_pixel(j*FRAME_WIDTH+i, color);
      }

      //printf("Setting pixel (%0d)=%0x\n", j*FRAME_WIDTH+i, color);
      //set_pixel(frame, j * FRAME_WIDTH + i, color);
    }
    printf("row %0d\n", j);
  }

  printf("Done generating frame\n");

  //dump_frame(frame);
  generate_ppm();

  return 0;
}
