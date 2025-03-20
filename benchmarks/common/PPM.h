#ifndef PPM_H_
#define PPM_H_

#include "stdint.h"

#define PPM_STATUS 0x4000
#define PPM_OPERATION 0x4004
#define PPM_ADDRESS 0x4008
#define PPM_DATA 0x400C
#define PPM_DUMP 0x4010
#define PPM_DOORBELL 0x4014

#define FRAME_WIDTH 100
#define FRAME_HEIGHT 100

#define CHAR_WIDTH 8
#define CHAR_HEIGHT 8

void set_pixel(uint32_t *frame, int address, unsigned int RGB);
void set_char(uint32_t *frame, int x, int y, const unsigned char *bitmap);
void write_pixel(uint32_t address, uint32_t data); // write to ppm module
void write_frame(uint32_t* frame);
void generate_ppm();
void dump_frame(uint32_t *frame);
void write_filename(char* filename);


#endif