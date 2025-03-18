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



int main(void)
{
    render_char(10, 10, font8x8_basic['H']);
    render_char(20, 10, font8x8_basic['E']);
    render_char(30, 10, font8x8_basic['L']);
    render_char(40, 10, font8x8_basic['L']);
    render_char(50, 10, font8x8_basic['O']);
    render_char(60, 10, font8x8_basic['!']);

    dump_frame();

    //update_frame();

    return 0;
}
