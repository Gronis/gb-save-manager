
#include "types.h"
#include "hardware.h"

#define INCLUDE_BIN_DATA_DONT_USE_IN_HEADER
#include "gb-companion_gbc.h"

void execute_vram_code(void);

void main(void) {
    // Must disable screen in order to write to VRAM
    *rLCDC = 0;
    uint8_t* src = gb_companion;
    uint8_t* end = src + gb_companion_length;
    uint8_t* dst = _VRAM;
    while(src != end){
        *dst = *src;
        if (*dst == *src){
            dst++;
            src++;
        }
    }
    execute_vram_code();
}