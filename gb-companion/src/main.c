#include "types.h"
#include "hardware.h"

#include "definitions.h"
#include "transfer.h"
#include "start.h"
#include "graphics.h"
#include "bitmaps.h"

const uint8_t corners_x[] = {
    0, SCREEN_COORDINATE_TILE_WIDTH, SCREEN_COORDINATE_TILE_WIDTH, 0
};

const uint8_t corners_y[] = {
    0, 0, SCREEN_COORDINATE_TILE_HEIGHT, SCREEN_COORDINATE_TILE_HEIGHT
};

#ifdef VRAM_VERSION
#include "ram_code_gbc.h"
void copy_ram_functions_to_ram(void) { 
    uint8_t* src = ram_code;
    volatile uint8_t* dst = (uint8_t*)RAM_LOC;
    uint8_t* end = src + ram_code_length;
    bool success = true;
    // If RAM becomes inaccessable during copy process, stall
    for (; src != end; dst+=success, src+=success){
        uint8_t val = *src;
        *dst = val;
        success = *dst == val;
    }
}
#else
// This is the RAM version, so no copy is necessary
void copy_ram_functions_to_ram(void) {}
#endif 

void main(void) {
    rasterize_tiles(&smiley, &smiley_bitmap);
    rasterize_tiles(&TEXT_ROLE, (tile_bitmap_t*)&text_role);
    {
        uint8_t tile_id = 1;
        bool did_write_to_ram = false;

        set_tiles_row(corners_x[3], corners_y[3], tiles[tile_id]);
        // Infinite loop to keep the program running
        while (1) {
            tile_id = (tile_id + 1) & 3;
            // toggle each corner except 1, flushing the screen in between to avoid flickering
            flush_screen();
            for (uint8_t i = 0; i < 3; ++i){
                set_tiles_row_repeat(corners_x[i], corners_y[i], tiles[tile_id], 3);
                flush_screen();
            }
            if(!did_write_to_ram){
                *((uint8_t*)_RAM+100) = 0x44;
                if (*((uint8_t*)_RAM+100) == 0x44) {
                    did_write_to_ram = true;
                    copy_ram_functions_to_ram();
                    run_in_parallel_to_screen(ram_tile_to_checker);
                }
            }
            // If RAM is unreadable, filcker slow, otherwise filcker fast.
            // RAM is unreadable when GBA is in GBC mode while no GBC cartridge is inserted. 
            if (*((uint8_t*)_RAM+100) != 0x44) {
                // wait for 24 frames
                for (uint8_t i = 0; i < 12; ++i){
                    flush_screen();
                }
            }
        }
    }
}
