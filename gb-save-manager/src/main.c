
#include "types.h"
#include "hardware.h"

#define INCLUDE_BIN_DATA_DONT_USE_IN_HEADER
#include "gb-companion_gbc.h"
#include "gb-companion_ram_gbc.h"

extern const uint8_t rDeviceModeBootup;

void execute_vram_code(void);
void execute_ram_code(void);

void mem_copy_with_validation(uint8_t* src, uint8_t* end, uint8_t* dst){
    while(src != end){
        *dst = *src;
        if (*dst == *src){
            dst++;
            src++;
        }
    }
}

void copy_vram_code(void) {
    uint8_t* src = gb_companion;
    uint8_t* end = src + gb_companion_length;
    uint8_t* dst = _VRAM;
    mem_copy_with_validation(src, end, dst);
}

void copy_ram_code(void) {
    uint8_t* src = gb_companion_ram;
    uint8_t* end = src + gb_companion_ram_length;
    uint8_t* dst = _RAM;
    mem_copy_with_validation(src, end, dst);
}

void main(void) {
    // Must disable screen in order to write to VRAM
    *rLCDC = 0;
    // bool is_device_agb = true;
    bool is_device_agb = (rDeviceModeBootup & (BOOTUP_B_AGB << 1)) != 0;
    // Use vram version for AGB device because it cannot interact with WRAM
    // without a game cartridge
    if (is_device_agb){
        copy_vram_code();
        execute_vram_code();
    } else {
        copy_ram_code();
        execute_ram_code();
    }

}