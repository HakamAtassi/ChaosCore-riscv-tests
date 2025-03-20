#include "mmio.h"
#include "PPM.h"
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


uint32_t frame[FRAME_HEIGHT * FRAME_WIDTH];
int main(void)
{

    // Frame buffer (RGB format, 32-bit per pixel)

    set_char(frame, 10, 10, font8x8_basic['H']);
    set_char(frame, 20, 10, font8x8_basic['E']);
    set_char(frame, 30, 10, font8x8_basic['L']);
    set_char(frame, 40, 10, font8x8_basic['L']);
    set_char(frame, 50, 10, font8x8_basic['O']);
    set_char(frame, 60, 10, font8x8_basic['!']);

    printf("Dumping frame\n");
    dump_frame(frame);

    //update_frame();

    return 0;
}
