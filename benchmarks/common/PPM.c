#include "mmio.h"
#include "PPM.h"
#include "stdint.h"


// convention:
// set_* -> write to local frame buffer
// write_* -> write to ppm module (via mmio commands)


void set_pixel(uint32_t *frame, int address, unsigned int RGB)
{
    frame[address] = RGB;
}

void set_char(uint32_t *frame, int x, int y, const unsigned char *bitmap)
{
    for (int row = 0; row < CHAR_HEIGHT; row++)
    {
        for (int col = 0; col < CHAR_WIDTH; col++)
        {
            if (bitmap[row] & (1 << col))
            {
                set_pixel(frame, (y + row) * FRAME_WIDTH + (x + col), 0xFFFFFF);
            }
        }
    }
}

void write_pixel(uint32_t address, uint32_t data) // write to ppm module
{
    // colors pixel at address with data in PPM periphiral
    reg_write32(PPM_OPERATION, 1);
    reg_write32(PPM_DATA, data);
    reg_write32(PPM_ADDRESS, address);
    reg_write32(PPM_DOORBELL, 1);
}

void write_frame(uint32_t* frame){
    for (int i = 0; i < FRAME_WIDTH * FRAME_HEIGHT; i++)
    {
        write_pixel(i, frame[i]);
    }
}

void generate_ppm() // dumps ppm module memory content to ppm file (simulation)
{
    reg_write32(PPM_DUMP, 1);
}

void dump_frame(uint32_t *frame) // moves frame buffer from core to device
{
    while (reg_read8(PPM_STATUS) == 0);
    write_frame(frame); // write frame to periphiral
    reg_write32(PPM_DUMP, 1);   // request dump to ppm file
    generate_ppm(); // request memory to .ppm dump
}

void write_filename(char* filename) // sets filename register
{

    int i=0;
    reg_write32(PPM_OPERATION, 2);

    while(filename[i]!='\0'){

        reg_write32(PPM_DATA, filename[i]);
        reg_write32(PPM_ADDRESS, i);
        reg_write32(PPM_DOORBELL, 1);
        i+=1;
    }
    generate_ppm();
}

