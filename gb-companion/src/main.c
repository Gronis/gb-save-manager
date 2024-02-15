#include "types.h"
#include "hardware.h"

#include "definitions.h"
#include "transfer.h"
#include "start.h"
#include "graphics.h"
#include "graphics_init.h"
#include "bitmaps.h"

// TODO: Move this to its own source file
////////////////////////////////////////////////////////////////////
#ifdef VRAM_VERSION
#define INCLUDE_BIN_DATA
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
////////////////////////////////////////////////////////////////////

const uint8_t corners_x[] = {
    0, SCREEN_COORDINATE_TILE_WIDTH, SCREEN_COORDINATE_TILE_WIDTH, 0
};

const uint8_t corners_y[] = {
    SCREEN_COORDINATE_TILE_HEIGHT - 1, 0, SCREEN_COORDINATE_TILE_HEIGHT, SCREEN_COORDINATE_TILE_HEIGHT
};

void main(void) {
    render_message_no_screen_flush_call_only_before_rasterize(message_header);
    rasterize_all_bitmap_tiles_to_VRAM();
    render_message(message_choose_action);
    render_message(message_role_leader);
    render_message(message_cartridge_state_ok);
    render_message(message_link_cable_state_ok);

    {
        uint8_t tile_id = 1;
        bool did_write_to_ram = false;

        {
            range_t* range = ((range_t*)(&tiles + tile_id));
            set_tiles_row(corners_x[3], corners_y[3], *range);
        }
        // Infinite loop to keep the program running
        while (1) {
            tile_id = (tile_id + 1);
            if (tile_id > 24){
                tile_id = 0;
            }
            // toggle each corner except 1, flushing the screen in between to avoid flickering
            for (uint8_t i = 0; i < 3; ++i){
                range_t* range = ((range_t*)(&tiles + tile_id));
                uint8_t width = range->end - range->start;
                flush_screen();
                set_tiles_row_repeat(corners_x[i], corners_y[i], *range, width);
                flush_screen();
            }
            if(!did_write_to_ram){
                *((uint8_t*)_RAM+400) = 0x44;
                if (*((uint8_t*)_RAM+400) == 0x44) {
                    did_write_to_ram = true;
                    copy_ram_functions_to_ram();
                    run_in_parallel_to_screen(ram_tile_to_checker);
                    // ram_tile_to_checker();
                }
            }
            // If RAM is unreadable, filcker slow, otherwise filcker fast.
            // RAM is unreadable when GBA is in GBC mode while no GBC cartridge is inserted. 
            if (*((uint8_t*)_RAM+400) != 0x44) {
                // wait for 24 frames
                for (uint8_t i = 0; i < 12; ++i){
                    flush_screen();
                }
            }
        }
    }
}
