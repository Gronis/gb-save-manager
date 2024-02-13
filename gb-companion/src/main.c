#include "types.h"
#include "hardware.h"

#include "definitions.h"
#include "transfer.h"
#include "start.h"

// Tiles only use 1-bit per pixel (no shades of gray)
typedef struct {
    uint8_t line[8];
} tile_t;

#define TILE_BASE_X ((uint8_t)(32 - 17))
#define TILE_BASE_Y ((uint8_t)(32 - 10))
#define SCREEN_TILE_WIDTH ((uint8_t)(14))
#define SCREEN_TILE_HEIGHT ((uint8_t)(8)) 
// BG has offsest 1/2 of a tile so 1 tile goes missing for some top+bottom margin

const uint8_t corners_x[] = {
    0, SCREEN_TILE_WIDTH, SCREEN_TILE_WIDTH, 0
};

const uint8_t corners_y[] = {
    0, 0, SCREEN_TILE_HEIGHT, SCREEN_TILE_HEIGHT
};

const tile_t smiley = {
    {// line:
        ~0x3C,  //   ####
        ~0x42,  //  #    #
        ~0xA5,  // # #  # #
        ~0x81,  // #      #
        ~0xA5,  // # #  # #
        ~0x99,  // #  ##  #
        ~0x42,  //  #    #
        ~0x3C,  //   ####
    }
};

void rasterize_tile(uint8_t id, tile_t* tile) {
    uint16_t _id = ((uint16_t)id) * 16;
    for (uint8_t i = 0; i < 8; i++){
        uint8_t line = tile->line[i];
        _VRAM[_id] = line;
        // Skip second byte (colors 0/2). Can be any value because of palette
        _id+= 2;
    }
}

void set_bkg_tiles(uint8_t x, uint8_t y, uint8_t width, uint8_t height, const uint8_t* tiles) {
    x += TILE_BASE_X;
    y += TILE_BASE_Y;
    uint8_t row, col;
    uint16_t index;
    uint8_t row_times_width = 0;
    for (row = 0; row < height; row++) {
        for (col = 0; col < width; col++) {
            // Calculate the index in the BG Map based on (x, y) coordinates
            index = (y + row) * 32 + (x + col);
            _SCRN1[index] = tiles[row_times_width + col];
        }
        row_times_width += width;
    }
}

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
    rasterize_tile(1, &smiley);
    {
        uint8_t tile_id = 1;
        bool did_write_to_ram = false;

        set_bkg_tiles(corners_x[3], corners_y[3], 1, 1, &tile_id);
        // Infinite loop to keep the program running
        while (1) {
            tile_id = (tile_id + 1) & 1;
            // toggle each corner except 1, flushing the screen in between to avoid flickering
            for (uint8_t i = 0; i < 3; ++i){
                set_bkg_tiles(corners_x[i], corners_y[i], 1, 1, &tile_id);
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
